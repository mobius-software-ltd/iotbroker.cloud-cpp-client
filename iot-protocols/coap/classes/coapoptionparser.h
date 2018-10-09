#ifndef COAPOPTIONPARSER_H
#define COAPOPTIONPARSER_H

#include "iot-protocols/coap/classes/coapenums.h"
#include <QByteArray>
#include <QVariant>
#include "iot-protocols/coap/classes/coapoption.h"
class CoapOptionParser
{
private:
    static QByteArray encodeMethod(CoAPOptionDefinitions option, QVariant object);
    static QVariant decodeMethod(CoAPOptionDefinitions option, QByteArray bytes);

public:

    static CoapOption encode(CoAPOptionDefinitions option, QVariant object);
    static QVariant decode(CoapOption option);

};

#endif // COAPOPTIONPARSER_H
