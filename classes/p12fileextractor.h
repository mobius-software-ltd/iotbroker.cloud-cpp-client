#ifndef P12FILEEXTRACTOR_H
#define P12FILEEXTRACTOR_H

#include <QObject>

class P12FileExtractor
{
private:
    char *fileName;
    char *password;

    char *certificate;
    char *caCertificate;

    void init();

    static bool isFileExist(char *fileName);

public:
    P12FileExtractor(char *fileName, char *password);

    char *getCertificate() const;
    char *getCaCertificate() const;

};

#endif // P12FILEEXTRACTOR_H
