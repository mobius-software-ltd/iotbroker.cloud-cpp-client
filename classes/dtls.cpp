#include "dtls.h"

#define MAXBUF 1024

Dtls *Dtls::m_dtls = nullptr;

Dtls::Dtls()
{
    this->isRun = true;
    this->tindex = 0;
    this->tid = (pthread_t *)malloc(sizeof(pthread_t));
    m_dtls = this;
}

static int min (int a, int b)
{
    return a > b ? b : a;
}

int dtls_sendto_cb(WOLFSSL* ssl, char* buf, int sz, void* ctx)
{
    Q_UNUSED(ssl);

    SharedDtls* shared = (SharedDtls*)ctx;
    return sendto(shared->sd, buf, sz, 0, (const struct sockaddr*)&shared->servAddr, shared->servSz);
}

int dtls_recvfrom_cb(WOLFSSL* ssl, char* buf, int sz, void* ctx)
{
    Q_UNUSED(ssl);

    SharedDtls* shared = (SharedDtls*)ctx;
    int copied;

    if (!shared->handShakeDone) {
        /* get directly from socket */
        return recvfrom(shared->sd, buf, sz, 0, NULL, NULL);
    } else {
        /* get the "pushed" datagram from our cb buffer instead */
        copied = min(sz, shared->recvSz);
        memcpy(buf, shared->recvBuf, copied);
        shared->recvSz -= copied;
        return copied;
    }
}

/* DTLS Send function in its own thread */
void* Dtls::datagramSend(void* arg)
{
    SharedDtls* shared = (SharedDtls*)arg;
    WOLFSSL*    ssl = shared->ssl;

    wc_LockMutex(&shared->shared_mutex);
    if ((wolfSSL_write(ssl, shared->sndBuf, shared->sndSz)) != shared->sndSz) {
        m_dtls->error((char *)"Error while send the message.");
    }
    wc_UnLockMutex(&shared->shared_mutex);

    return NULL;
}

void Dtls::setHost(char *host, int port)
{
    this->host = host;
    this->port = port;
}

void Dtls::setCerts(char *caCert, char *cert)
{
    this->caCertificate = caCert;
    this->certificate = cert;
}

void Dtls::start()
{
    int     	 sockfd = 0;
    WOLFSSL* 	 ssl = 0;
    WOLFSSL_CTX* ctx = 0;

    int          sz = 0;
    char         recvBuf[MAXBUF];
    char         plainBuf[MAXBUF];
    int          err1;

    recvShared = &shared;

    wolfSSL_Init();

    if ((ctx = wolfSSL_CTX_new(wolfDTLSv1_2_client_method())) == NULL) {
        emit error((char *)"Dtls initialize error.");
        return;
    }

    const unsigned char* ca = reinterpret_cast<const unsigned char *>(this->caCertificate);

    if (wolfSSL_CTX_load_verify_buffer(ctx, ca, strlen(this->caCertificate), SSL_FILETYPE_PEM) != SSL_SUCCESS) {
        char message[64];
        sprintf(message, "Error while loading CA Certificete file");
        emit error(message);
        return;
    }

    wolfSSL_SetIOSend(ctx, dtls_sendto_cb);
    wolfSSL_SetIORecv(ctx, dtls_recvfrom_cb);

    ssl = wolfSSL_new(ctx);
    if (ssl == NULL) {
        emit error((char *)"SSL initialize error.");
        return;
    }

    memset(&shared.servAddr, 0, sizeof(shared.servAddr));
    shared.servAddr.sin_family = AF_INET;
    shared.servAddr.sin_port = htons(this->port);
    if (inet_pton(AF_INET, this->host, &shared.servAddr.sin_addr) < 1) {
        emit error((char *)"Error and/or invalid IP address.");
        return;
    }

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        emit error((char *)"Cannot create a socket.");
        return;
    }

    shared.sd = sockfd;
    shared.servSz = sizeof(shared.servAddr);
    shared.ssl = ssl;
    shared.handShakeDone = 0;

    if (wc_InitMutex(&shared.shared_mutex) != 0) {
        emit error((char *)"Mutex initialize error.");
        return;
    }

    wolfSSL_SetIOWriteCtx(ssl, &shared);
    wolfSSL_SetIOReadCtx(ssl, &shared);

    if (wolfSSL_connect(ssl) != SSL_SUCCESS) {
        err1 = wolfSSL_get_error(ssl, 0);
        emit error((char *)"SSL connect error.");
        return;
    }

    emit didConnect();

    shared.handShakeDone = 1;

    /* DTLS Recv */
    while(this->isRun) {
        /* first get datagram, works in blocking mode too */
        sz = recvfrom(recvShared->sd, recvBuf, MAXBUF, 0, NULL, NULL);

        wc_LockMutex(&recvShared->shared_mutex);
        recvShared->recvBuf = recvBuf;
        recvShared->recvSz = sz;

        if (this->isRun) {
            if ( (sz = (wolfSSL_read(ssl, plainBuf, MAXBUF-1))) < 0) {
                emit error((char *)"Error while read the message.");
            }
        }
        wc_UnLockMutex(&recvShared->shared_mutex);
        plainBuf[MAXBUF-1] = '\0';

        emit received(plainBuf);
    }

    for (int i = 0; i < this->tindex + 1; i++) {
        pthread_join(this->tid[i], NULL);
    }

    emit didDisconnect();

    wolfSSL_shutdown(ssl);
    wolfSSL_free(ssl);
    close(sockfd);
    wolfSSL_CTX_free(ctx);
    wc_FreeMutex(&shared.shared_mutex);
    wolfSSL_Cleanup();
}

void Dtls::send(char *message)
{
    pthread_t tid;

    shared.sndBuf = message;
    shared.sndSz = strlen(shared.sndBuf) + 1;

    pthread_create(&tid, NULL, datagramSend, &shared);
    this->tid[this->tindex] = tid;

    this->tindex++;
    this->tid = (pthread_t *)realloc(this->tid, (this->tindex + 1) * sizeof(pthread_t));
}

void Dtls::stop()
{
    this->isRun = false;
    close(this->recvShared->sd);
}
