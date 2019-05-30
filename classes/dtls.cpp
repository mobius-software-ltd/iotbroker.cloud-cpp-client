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

static char* hostname_to_ip(char * hostname)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;

    if ( (he = gethostbyname( hostname ) ) == NULL)
    {
        // get the host info
        printf("Cannot get ip from hostname : %s\n", hostname);
        return NULL;
    }

    addr_list = (struct in_addr **) he->h_addr_list;

    for(i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        return inet_ntoa(*addr_list[i]);
    }

    return NULL;
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

void Dtls::setHost(char *host, int port)
{
    this->host = host;
    this->port = port;
}

void Dtls::setCertsAndKey(QString cert, QString key)
{
    this->certificate = cert;
    this->key = key;
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
    shared.ctx = ctx;

    wolfSSL_CTX_set_verify(ctx,SSL_VERIFY_NONE, NULL);
    if (!this->certificate.isNull() && !this->certificate.isEmpty()) {
        char* certSequence;
        certSequence = new char[this->certificate.size()+1];
        strcpy(certSequence, this->certificate.toLocal8Bit());

        const unsigned char* certs = reinterpret_cast<const unsigned char *>(certSequence);
        if (wolfSSL_CTX_use_certificate_chain_buffer_format(ctx, certs, strlen((char *)certs), SSL_FILETYPE_PEM) != SSL_SUCCESS) {
            char message[64];
            sprintf(message, "Error while loading Certificates ");
            emit error(message);
            return;
        }
    }
    if (!this->key.isNull() && !this->key.isEmpty()) {
        char* keySequence;
        keySequence = new char[this->key.size()+1];
        strcpy(keySequence, this->key.toLocal8Bit());

        const unsigned char* k = reinterpret_cast<const unsigned char *>(keySequence);
        if (wolfSSL_CTX_use_PrivateKey_buffer(ctx, k, strlen((char *)k), SSL_FILETYPE_PEM) != SSL_SUCCESS) {
            char message[64];
            sprintf(message, "Error while loading Key of certificate");
            emit error(message);
            return;
        }
    }
    wolfSSL_SetIOSend(ctx, dtls_sendto_cb);
    wolfSSL_SetIORecv(ctx, dtls_recvfrom_cb);

    ssl = wolfSSL_new(ctx);
    if (ssl == NULL) {
        emit error((char *)"SSL initialize error.");
        return;
    }
    char * remote_ip = hostname_to_ip(this->host);
    if(remote_ip == NULL) {
        emit error((char *)"Cannot get ip from domen name.");
        return;
    }
    memset(&shared.servAddr, 0, sizeof(shared.servAddr));
    shared.servAddr.sin_family = AF_INET;
    shared.servAddr.sin_port = htons(this->port);
    if (inet_pton(AF_INET, remote_ip, &shared.servAddr.sin_addr) < 1) {
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

//    if (wc_InitMutex(&shared.shared_mutex) != 0) {
//        emit error((char *)"Mutex initialize error.");
//        return;
//    }

    wolfSSL_SetIOWriteCtx(ssl, &shared);
    wolfSSL_SetIOReadCtx(ssl, &shared);

    int result = wolfSSL_connect(ssl);
    if (result != SSL_SUCCESS) {
        err1 = wolfSSL_get_error(ssl, 0);
        emit error((char *)"SSL connect error.");
        return;
    }

    emit didConnect();

    shared.handShakeDone = 1;

    /* DTLS Recv */
    struct timeval read_timeout;
    read_timeout.tv_sec = 1;
    read_timeout.tv_usec = 10;
    setsockopt(shared.sd, SOL_SOCKET, SO_RCVTIMEO, &read_timeout, sizeof read_timeout);
    while(this->isRun) {
        /* first get datagram, works in blocking mode too */
        sz = recvfrom(shared.sd, recvBuf, MAXBUF, 0, NULL, NULL);
        if(sz<0)
            continue;

        recvShared->recvBuf = recvBuf;
        recvShared->recvSz = sz;

        if (this->isRun) {
            if ( (sz = (wolfSSL_read(ssl, plainBuf, MAXBUF-1))) < 0) {
                emit error((char *)"Error while read the message.");
            } else {
                plainBuf[MAXBUF-1] = '\0';
                emit received(plainBuf);
            }
        }

    }

}

void Dtls::send(QByteArray message)
{
    if(this->isRun)
        wolfSSL_write(shared.ssl, message.data(), message.size());
}

void Dtls::stop()
{
    this->isRun = false;
    wolfSSL_free(shared.ssl);      /* Free the wolfSSL object                  */
    wolfSSL_CTX_free(shared.ctx);  /* Free the wolfSSL context object          */
    wolfSSL_Cleanup();      /* Cleanup the wolfSSL environment          */
    close(this->recvShared->sd);
//    wc_FreeMutex(&shared.shared_mutex);

    emit didDisconnect();
}
