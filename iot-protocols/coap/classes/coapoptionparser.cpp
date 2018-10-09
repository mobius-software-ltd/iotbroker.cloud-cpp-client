#include "coapoptionparser.h"
#include "classes/bytearray.h"
#include <QDebug>

CoapOption CoapOptionParser::encode(CoAPOptionDefinitions option, QVariant object)
{
    QByteArray value = CoapOptionParser::encodeMethod(option, object);
    return CoapOption(option, value.length(), value);
}

QVariant CoapOptionParser::decode(CoapOption option)
{
    return CoapOptionParser::decodeMethod((CoAPOptionDefinitions)option.getNumber(), option.getValue());
}

QByteArray CoapOptionParser::encodeMethod(CoAPOptionDefinitions option, QVariant object)
{
    ByteArray array = ByteArray();

    switch (option) {
    case COAP_URI_PORT_OPTION:
    case COAP_CONTENT_FORMAT_OPTION:
    case COAP_ACCEPT_OPTION: {
        int value = object.toInt();
        array.writeChar(0x00);
        array.writeChar((unsigned char)value);
    } break;
    case COAP_MAX_AGE_OPTION:
    case COAP_SIZE1_OPTION:
    case COAP_OBSERVE_OPTION: {
        int value = object.toInt();
        array.writeInt((unsigned char)value);
    } break;
    case COAP_NODE_ID_OPTION:
    case COAP_IF_MATCH_OPTION:
    case COAP_URI_HOST_OPTION:
    case COAP_ETAG_OPTION:
    case COAP_URI_PATH_OPTION:
    case COAP_LOCATION_PATH_OPTION:
    case COAP_URI_QUERY_OPTION:
    case COAP_LOCATION_QUERY_OPTION:
    case COAP_PROXY_SCHEME_OPTION:
    case COAP_PROXY_URI_OPTION: {
        array = object.toString().toUtf8();
    } break;
    default:
        break;
    }

    return array.getByteArray();
}

QVariant CoapOptionParser::decodeMethod(CoAPOptionDefinitions option, QByteArray bytes)
{
    QVariant variant = QVariant();
    ByteArray array = ByteArray(QByteArray(bytes));

    switch (option) {
    case COAP_URI_PORT_OPTION:
    case COAP_CONTENT_FORMAT_OPTION:
    case COAP_ACCEPT_OPTION: {
        if (array.getSize() >= 2) {
            variant = QVariant(array.readShort());
        }
    } break;
    case COAP_MAX_AGE_OPTION:
    case COAP_SIZE1_OPTION:
    case COAP_OBSERVE_OPTION: {
        if (array.getSize() >= 4) {
            variant = QVariant(array.readInt());
        }
    } break;
    case COAP_NODE_ID_OPTION:
    case COAP_IF_MATCH_OPTION:
    case COAP_URI_HOST_OPTION:
    case COAP_ETAG_OPTION:
    case COAP_URI_PATH_OPTION:
    case COAP_LOCATION_PATH_OPTION:
    case COAP_URI_QUERY_OPTION:
    case COAP_LOCATION_QUERY_OPTION:
    case COAP_PROXY_SCHEME_OPTION:
    case COAP_PROXY_URI_OPTION: {
        variant = QVariant(QString(bytes));
    } break;
    default:
        break;
    }

    return variant;
}
