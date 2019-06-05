#ifndef DTLS_H
#define DTLS_H

#include <QObject>
#include <wolfssl/options.h>
#include <wolfssl/ssl.h>
#include <wolfssl/wolfcrypt/wc_port.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct SharedDtls {
    wolfSSL_Mutex      shared_mutex;  /* mutex for using */
    WOLFSSL*           ssl;           /* WOLFSSL object being shared */
    WOLFSSL_CTX*       ctx;
    int                sd;            /* socket fd */
    struct sockaddr_in servAddr;      /* server sockaddr */
    socklen_t          servSz;        /* length of servAddr */
    char*              recvBuf;       /* I/O recv cb buffer */
    int                recvSz;          /* bytes in recvBuf */
    int                handShakeDone;   /* is the handshake done? */
    QByteArray         sndBuf;
} SharedDtls;

class Dtls : public QObject
{
    Q_OBJECT

    static Dtls *m_dtls;

    char *host;
    int port;

    QString certificate;
    QString key;

    SharedDtls shared;
    SharedDtls* recvShared;

    int tindex;
    pthread_t *tid;

    bool isRun;

public:
    Dtls();

    static void* datagramSend(void* arg);

    void setHost(char *host, int port);
    void setCertsAndKey(QString cert, QString key);

    void start();
    void send(QByteArray message);
    void stop();

signals:
    void didConnect();
    void didDisconnect();
    void received(char *message, int size);
    void error(char *message);
};

#endif // DTLS_H
