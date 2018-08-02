#include "mqjsonparser.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "iot-protocols/mqtt/classes/mqttenums.h"
#include "iot-protocols/mqtt/messages/connect.h"
#include "iot-protocols/mqtt/messages/connack.h"
#include "iot-protocols/mqtt/messages/disconnect.h"
#include "iot-protocols/mqtt/messages/pingreq.h"
#include "iot-protocols/mqtt/messages/pingresp.h"
#include "iot-protocols/mqtt/messages/publish.h"
#include "iot-protocols/mqtt/messages/puback.h"
#include "iot-protocols/mqtt/messages/pubcomp.h"
#include "iot-protocols/mqtt/messages/pubrec.h"
#include "iot-protocols/mqtt/messages/pubrel.h"
#include "iot-protocols/mqtt/messages/subscribe.h"
#include "iot-protocols/mqtt/messages/suback.h"
#include "iot-protocols/mqtt/messages/unsubscribe.h"
#include "iot-protocols/mqtt/messages/unsuback.h"

QByteArray MQJsonParser::json(Message *message)
{
    QJsonObject obj = QJsonObject();

    MQMessageType type = (MQMessageType)message->getType();

    switch (type) {

    case MQ_CONNECT: {
        Connect *connect = (Connect *)message;
        obj["packet"] = connect->getType();
        obj["protocolLevel"] = connect->getProtocolLevel();
        obj["protocolName"] = connect->getProtocol()->getName();
        obj["username"] = connect->getUsername();
        obj["password"] = connect->getPassword();
        obj["clientID"] = connect->getClientID();
        obj["cleanSession"] = connect->isCleanSession();
        obj["keepalive"] = connect->getKeepAlive();
        obj["willFlag"] = connect->isWillFlag();
        obj["usernameFlag"] = connect->isUsernameFlag();
        obj["passwordFlag"] = connect->isPasswordFlag();
        if (connect->getWill()) {
            QJsonObject topic = QJsonObject();
            topic["name"] = connect->getWill()->getTopic()->getName();
            topic["qos"] = connect->getWill()->getTopic()->getQoS()->getValue();
            QJsonObject will = QJsonObject();
            will["topic"] = topic;
            will["content"] = QString(connect->getWill()->getContent().toBase64());
            will["retain"] = connect->getWill()->isRetain();
            obj["will"] = will;
        }
    } break;
    case MQ_CONNACK: {
        Connack *connack = (Connack *)message;
        obj["packet"] = connack->getType();
        obj["sessionPresent"] = connack->isSessionPresent();
        obj["returnCode"] = connack->getReturnCode();
    } break;
    case MQ_PUBLISH: {
        Publish *publish = (Publish *)message;
        obj["packet"] = publish->getType();
        obj["packetID"] = publish->getPacketID();
        QJsonObject topic = QJsonObject();
        topic["name"] = publish->getTopic()->getName();
        topic["qos"] = publish->getTopic()->getQoS()->getValue();
        obj["topic"] = topic;
        obj["content"] = QString(publish->getContent().toBase64());
        obj["retain"] = publish->isRetain();
        obj["dup"] = publish->isDup();
    } break;
    case MQ_PUBACK: {
        Puback *puback = (Puback *)message;
        obj["packet"] = puback->getType();
        obj["packetID"] = puback->getPacketID();
    } break;
    case MQ_PUBREC: {
        Pubrec *pubrec = (Pubrec *)message;
        obj["packet"] = pubrec->getType();
        obj["packetID"] = pubrec->getPacketID();
    } break;
    case MQ_PUBREL: {
        Pubrel *pubrel = (Pubrel *)message;
        obj["packet"] = pubrel->getType();
        obj["packetID"] = pubrel->getPacketID();
    } break;
    case MQ_PUBCOMP: {
        Pubcomp *pubcomp = (Pubcomp *)message;
        obj["packet"] = pubcomp->getType();
        obj["packetID"] = pubcomp->getPacketID();
    } break;
    case MQ_SUBSCRIBE: {
        Subscribe *subscribe = (Subscribe *)message;
        obj["packet"] = subscribe->getType();
        obj["packetID"] = subscribe->getPacketID();
        QJsonArray topics = QJsonArray();
        for (int i = 0; i < subscribe->getTopics()->size(); i++) {
            MQTopic topic = subscribe->getTopics()->at(i);
            QJsonObject topicObj = QJsonObject();
            topicObj["name"] = topic.getName();
            topicObj["qos"] = topic.getQoS()->getValue();
            topics.append(topicObj);
        }
        obj["topics"] = topics;
    } break;
    case MQ_SUBACK: {
        Suback *suback = (Suback *)message;
        obj["packet"] = suback->getType();
        obj["packetID"] = suback->getPacketID();
        QJsonArray codes = QJsonArray();
        for (int i = 0; i < suback->getReturnCodes()->size(); i++) {
            codes.append(QJsonValue(suback->getReturnCodes()->at(i)));
        }
        obj["returnCodes"] = codes;
    } break;
    case MQ_UNSUBSCRIBE: {
        Unsubscribe *unsubscribe = (Unsubscribe *)message;
        obj["packet"] = unsubscribe->getType();
        obj["packetID"] = unsubscribe->getPacketID();
        QJsonArray topics = QJsonArray();
        for (int i = 0; i < unsubscribe->getTopics()->size(); i++) {
            topics.append(QJsonValue(unsubscribe->getTopics()->at(i)));
        }
        obj["topics"] = topics;
    } break;
    case MQ_UNSUBACK: {
        Unsuback *unsuback = (Unsuback *)message;
        obj["packet"] = unsuback->getType();
        obj["packetID"] = unsuback->getPacketID();
    } break;
    case MQ_PINGREQ: {
        Pingreq *pingreq = (Pingreq *)message;
        obj["packet"] = pingreq->getType();
    } break;
    case MQ_PINGRESP: {
        Pingresp *pingresp = (Pingresp *)message;
        obj["packet"] = pingresp->getType();
    } break;
    case MQ_DISCONNECT: {
        Disconnect *disconnect = (Disconnect *)message;
        obj["packet"] = disconnect->getType();
    } break;
    default:
        return QByteArray();
    }

    QJsonDocument doc(obj);
    return doc.toJson();
}

Message *MQJsonParser::message(QByteArray json)
{
    QJsonDocument doc = QJsonDocument::fromJson(json);
    QJsonObject obj = doc.object();

    if (obj.contains("packet")) {
        MQMessageType type = (MQMessageType)obj["packet"].toInt();

        switch (type) {

        case MQ_CONNECT: {
            Connect *connect = new Connect();
            connect->setUsername(obj["username"].toString());
            connect->setPassword(obj["password"].toString());
            connect->setClientID(obj["clientID"].toString());
            connect->setCleanSession(obj["cleanSession"].toBool());
            connect->setKeepAlive(obj["keepalive"].toInt());
            if (obj.contains("will")) {
                Will *will = new Will();
                QJsonObject willObj = obj["will"].toObject();
                if (willObj.contains("topic")) {
                    QJsonObject topicObj = willObj["topic"].toObject();
                    MQTopic *topic = new MQTopic(topicObj["name"].toString(), new QoS(topicObj["qos"].toInt()));
                    will->setTopic(topic);
                }
                will->setContent(QByteArray::fromBase64(willObj["content"].toString().toUtf8()));
                will->setRetain(willObj["retain"].toBool());
                connect->setWill(will);
            }
            return connect;
        } break;
        case MQ_CONNACK: {
            Connack *connack = new Connack();
            connack->setSessionPresent(obj["sessionPresent"].toBool());
            connack->setReturnCode((MQConnackCode)obj["returnCode"].toInt());
            return connack;
        } break;
        case MQ_PUBLISH: {
            Publish *publish = new Publish();
            publish->setPacketID(obj["packetID"].toInt());
            if (obj.contains("topic")) {
                QJsonObject topicObj = obj["topic"].toObject();
                MQTopic *topic = new MQTopic(topicObj["name"].toString(), new QoS(topicObj["qos"].toInt()));
                publish->setTopic(topic);
            }
            publish->setContent(QByteArray::fromBase64(obj["content"].toString().toUtf8()));
            publish->setRetain(obj["retain"].toBool());
            publish->setDup(obj["dup"].toBool());
            return publish;
        } break;
        case MQ_PUBACK: {
            Puback *puback = new Puback();
            puback->setPacketID(obj["packetID"].toInt());
            return puback;
        } break;
        case MQ_PUBREC: {
            Pubrec *pubrec = new Pubrec();
            pubrec->setPacketID(obj["packetID"].toInt());
            return pubrec;
        } break;
        case MQ_PUBREL: {
            Pubrel *pubrel = new Pubrel();
            pubrel->setPacketID(obj["packetID"].toInt());
            return pubrel;
        } break;
        case MQ_PUBCOMP: {
            Pubcomp *pubcomp = new Pubcomp();
            pubcomp->setPacketID(obj["packetID"].toInt());
            return pubcomp;
        } break;
        case MQ_SUBSCRIBE: {
            Subscribe *subscribe = new Subscribe();
            subscribe->setPacketID(obj["packetID"].toInt());
            if (obj.contains("topics")) {
                QList<MQTopic> *list = new QList<MQTopic>();
                QJsonArray topics = obj["topics"].toArray();
                for (int i = 0; i < topics.size(); i++) {
                    QJsonObject topicObj = topics.at(i).toObject();
                    MQTopic topic = MQTopic(topicObj["name"].toString(), new QoS(topicObj["qos"].toInt()));
                    list->append(topic);
                }
                subscribe->setTopics(list);
            }
            return subscribe;
        } break;
        case MQ_SUBACK: {
            Suback *suback = new Suback();
            suback->setPacketID(obj["packetID"].toInt());
            if (obj.contains("returnCodes")) {
                QList<SubackCode> *list = new QList<SubackCode>();
                QJsonArray codes = obj["returnCodes"].toArray();
                for (int i = 0; i < codes.size(); i++) {
                    SubackCode code = (SubackCode)codes.at(i).toInt();
                    list->append(code);
                }
                suback->setReturnCodes(list);
            }
            return suback;
        } break;
        case MQ_UNSUBSCRIBE: {
            Unsubscribe *unsubscribe = new Unsubscribe();
            unsubscribe->setPacketID(obj["packetID"].toInt());
            if (obj.contains("topics")) {
                QList<QString> *list = new QList<QString>();
                QJsonArray topics = obj["topics"].toArray();
                for (int i = 0; i < topics.size(); i++) {
                    QString topic = topics.at(i).toString();
                    list->append(topic);
                }
                unsubscribe->setTopics(list);
            }
            return unsubscribe;
        } break;
        case MQ_UNSUBACK: {
            Unsuback *unsuback = new Unsuback();
            unsuback->setPacketID(obj["packetID"].toInt());
            return unsuback;
        } break;
        case MQ_PINGREQ: {
            Pingreq *pingreq = new Pingreq();
            return pingreq;
        } break;
        case MQ_PINGRESP: {
            Pingresp *pingresp = new Pingresp();
            return pingresp;
        } break;
        case MQ_DISCONNECT: {
            Disconnect *disconnect = new Disconnect();
            return disconnect;
        } break;
        default:
            return NULL;
        }
    }

    return NULL;
}
