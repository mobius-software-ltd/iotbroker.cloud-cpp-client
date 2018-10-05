#ifndef COAPOPTIONPARSER_H
#define COAPOPTIONPARSER_H

#include "iot-protocols/coap/classes/coapenums.h"
#include <QByteArray>
#include <QVariant>

class CoapOptionParser
{
public:

    static QByteArray encode(CoAPOptionDefinitions option, QVariant object);
    static QVariant decode(CoAPOptionDefinitions option, QByteArray bytes);

};

#endif // COAPOPTIONPARSER_H
