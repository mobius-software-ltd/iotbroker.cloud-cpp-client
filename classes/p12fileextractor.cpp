#include "p12fileextractor.h"
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <sys/stat.h>
#include <openssl/x509.h>
#include "messageexception.h"
#include <stdio.h>
#include <stdlib.h>
#include <QDebug>

P12FileExtractor::P12FileExtractor(char *fileName, char *password)
{
    this->fileName = fileName;
    this->password = password;
    this->certificate = NULL;
    this->caCertificate = NULL;

    this->init();
}

char *P12FileExtractor::getCertificate() const
{
    return certificate;
}

char *P12FileExtractor::getCaCertificate() const
{
    return caCertificate;
}

char *buffer(BIO *bio_out, X509 *cert) {
    bio_out = BIO_new(BIO_s_mem());
    PEM_write_bio_X509(bio_out, cert);
    BUF_MEM *bio_buf;
    BIO_get_mem_ptr(bio_out, &bio_buf);
    char *buffer = bio_buf->data;
    buffer[bio_buf->length] = '\0';
    return buffer;
}

void P12FileExtractor::init()
{
    FILE *p12file;
    PKCS12 *p12certificate = NULL;
    EVP_PKEY *privateKey = NULL;
    X509 *cert = NULL;
    STACK_OF(X509) *caCerts = NULL;

    if (isFileExist(this->fileName)) {

        p12file = fopen(this->fileName, "rb");
        d2i_PKCS12_fp(p12file, &p12certificate);
        fclose(p12file);

        if (PKCS12_parse(p12certificate, this->password, &privateKey, &cert, &caCerts)) {
            BIO *bio_out = BIO_new(BIO_s_mem());
            this->certificate = buffer(bio_out, cert);
            int size = sk_X509_num(caCerts);
            if (size > 0) {
                this->caCertificate = buffer(bio_out, sk_X509_value(caCerts, 0));
            } else {
                qDebug() << "There is no CA Certificate";
            }
            BIO_free(bio_out);
        } else {
            throw MessageException("Error while p12 file parsing.");
        }
        PKCS12_free(p12certificate);
    } else {
        char message[64];
        sprintf(message, "Error while loading file: %s.", this->fileName);
        throw MessageException(QString(message));
    }
}

bool P12FileExtractor::isFileExist(char *fileName) {
    struct stat st;
    int result = stat(fileName, &st);
    return (result == 0);
}
