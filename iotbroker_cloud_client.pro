#-------------------------------------------------------------------
#
# Mobius Software LTD
# Copyright 2015-2018, Mobius Software LTD
#
# This is free software; you can redistribute it and/or modify it
# under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation; either version 2.1 of
# the License, or (at your option) any later version.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this software; if not, write to the Free
# Software Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301 USA, or see the FSF site: http://www.fsf.org.
#
#-------------------------------------------------------------------

#-------------------------------------------------
#
# Project created by QtCreator 2017-01-11T13:06:07
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = iotbroker_cloud_client
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(src/dquest.pri)

SOURCES += main.cpp\
        mainwindow.cpp \
    database/database.cpp \
    database/accountmanager.cpp \
    listwidget.cpp \
    cells/cellwithcheckbox.cpp \
    cells/cellwitheditline.cpp \
    cells/cellwithcombobox.cpp \
    tabs/topiclisttab.cpp \
    cells/celltopicitem.cpp \
    tabs/sendmessagetab.cpp \
    tabs/messageslisttab.cpp \
    cells/cellmessageitem.cpp \
    cells/cellaccountitem.cpp \
    timer/timertask.cpp \
    timer/timersmap.cpp \
    internet-protocols/internetprotocol.cpp \
    internet-protocols/tcpsocket.cpp \
    internet-protocols/udpsocket.cpp \
    iot-protocols/iotprotocol.cpp \
    iot-protocols/mqtt/mqtt.cpp \
    classes/bytearray.cpp \
    classes/lengthdetails.cpp \
    iot-protocols/classes/qos.cpp \
    iot-protocols/mqtt/classes/will.cpp \
    iot-protocols/mqtt/messages/connack.cpp \
    iot-protocols/mqtt/messages/connect.cpp \
    iot-protocols/mqtt/messages/disconnect.cpp \
    iot-protocols/mqtt/messages/pingreq.cpp \
    iot-protocols/mqtt/messages/pingresp.cpp \
    iot-protocols/mqtt/messages/puback.cpp \
    iot-protocols/mqtt/messages/pubcomp.cpp \
    iot-protocols/mqtt/messages/publish.cpp \
    iot-protocols/mqtt/messages/pubrec.cpp \
    iot-protocols/mqtt/messages/pubrel.cpp \
    iot-protocols/mqtt/messages/suback.cpp \
    iot-protocols/mqtt/messages/subscribe.cpp \
    iot-protocols/mqtt/messages/unsuback.cpp \
    iot-protocols/mqtt/messages/unsubscribe.cpp \
    iot-protocols/mqtt/parser/messagesparser.cpp \
    iot-protocols/mqtt/parser/parser.cpp \
    iot-protocols/mqtt-sn/mqttsn.cpp \
    iot-protocols/mqtt-sn/messages/snadvertise.cpp \
    iot-protocols/mqtt-sn/messages/snencapsulated.cpp \
    iot-protocols/mqtt-sn/messages/sngwinfo.cpp \
    iot-protocols/mqtt-sn/messages/snregack.cpp \
    iot-protocols/mqtt-sn/messages/snregister.cpp \
    iot-protocols/mqtt-sn/messages/snconnack.cpp \
    iot-protocols/mqtt-sn/messages/snconnect.cpp \
    iot-protocols/mqtt-sn/messages/sndisconnect.cpp \
    iot-protocols/mqtt-sn/messages/snpingreq.cpp \
    iot-protocols/mqtt-sn/messages/snpingresp.cpp \
    iot-protocols/mqtt-sn/messages/snpuback.cpp \
    iot-protocols/mqtt-sn/messages/snpubcomp.cpp \
    iot-protocols/mqtt-sn/messages/snpublish.cpp \
    iot-protocols/mqtt-sn/messages/snpubrec.cpp \
    iot-protocols/mqtt-sn/messages/snpubrel.cpp \
    iot-protocols/mqtt-sn/messages/snsuback.cpp \
    iot-protocols/mqtt-sn/messages/snsubscribe.cpp \
    iot-protocols/mqtt-sn/messages/snunsuback.cpp \
    iot-protocols/mqtt-sn/messages/snunsubscribe.cpp \
    iot-protocols/mqtt-sn/messages/snsearchgw.cpp \
    iot-protocols/mqtt-sn/messages/snwillmsg.cpp \
    iot-protocols/mqtt-sn/messages/snwillmsgreq.cpp \
    iot-protocols/mqtt-sn/messages/snwillmsgresp.cpp \
    iot-protocols/mqtt-sn/messages/snwillmsgupd.cpp \
    iot-protocols/mqtt-sn/messages/snwilltopic.cpp \
    iot-protocols/mqtt-sn/messages/snwilltopicreq.cpp \
    iot-protocols/mqtt-sn/messages/snwilltopicresp.cpp \
    iot-protocols/mqtt-sn/messages/snwilltopicupd.cpp \
    iot-protocols/mqtt/classes/mqtopic.cpp \
    iot-protocols/mqtt-sn/classes/topics/snfulltopic.cpp \
    iot-protocols/mqtt-sn/classes/topics/snidentifiertopic.cpp \
    iot-protocols/mqtt-sn/classes/topics/snshorttopic.cpp \
    iot-protocols/mqtt-sn/classes/snresponsemessage.cpp \
    iot-protocols/mqtt-sn/classes/snvaluesvalidator.cpp \
    iot-protocols/mqtt-sn/classes/sncontrols.cpp \
    iot-protocols/mqtt-sn/classes/snflags.cpp \
    iot-protocols/mqtt-sn/parser/snparser.cpp \
    iot-protocols/mqtt-sn/parser/snmessagesparser.cpp \
    iot-protocols/coap/coap.cpp \
    iot-protocols/coap/messages/coapmessage.cpp \
    iot-protocols/coap/parser/coapparser.cpp \
    iot-protocols/amqp/classes/enums/amqpdistributionmode.cpp \
    iot-protocols/amqp/classes/enums/enumobject.cpp \
    iot-protocols/amqp/classes/enums/amqperrorcode.cpp \
    iot-protocols/amqp/classes/enums/amqpheadercode.cpp \
    iot-protocols/amqp/classes/enums/amqplifetimepolicy.cpp \
    iot-protocols/amqp/classes/enums/amqpreceiversettlemode.cpp \
    iot-protocols/amqp/classes/enums/amqprolecode.cpp \
    iot-protocols/amqp/classes/enums/amqpsaslcode.cpp \
    iot-protocols/amqp/classes/enums/amqpsectioncode.cpp \
    iot-protocols/amqp/classes/enums/amqpsendcode.cpp \
    iot-protocols/amqp/classes/enums/amqpstatecode.cpp \
    iot-protocols/amqp/classes/enums/amqpterminusdurability.cpp \
    iot-protocols/amqp/classes/enums/amqpterminusexpirypolicy.cpp \
    iot-protocols/amqp/classes/enums/amqptype.cpp \
    iot-protocols/amqp/classes/enums/amqpconnectionproperty.cpp \
    iot-protocols/amqp/classes/enums/amqpdynamicnodeproperty.cpp \
    iot-protocols/amqp/classes/constructors/amqpsimpleconstructor.cpp \
    iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.cpp \
    iot-protocols/amqp/classes/tlv/api/tlvamqp.cpp \
    iot-protocols/amqp/classes/tlv/variable/amqptlvvariable.cpp \
    iot-protocols/amqp/classes/tlv/fixed/amqptlvnull.cpp \
    iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.cpp \
    iot-protocols/amqp/classes/tlv/array/amqptlvarray.cpp \
    iot-protocols/amqp/classes/tlv/compound/amqptlvmap.cpp \
    iot-protocols/amqp/classes/tlv/compound/amqptlvlist.cpp \
    iot-protocols/amqp/classes/tlv/described/amqpaccepted.cpp \
    iot-protocols/amqp/classes/tlv/described/amqplifetimepolicyobject.cpp \
    iot-protocols/amqp/classes/tlv/described/amqpreceived.cpp \
    iot-protocols/amqp/classes/amqpsimpletype.cpp \
    iot-protocols/amqp/classes/wrappers/messageID/amqpbinaryid.cpp \
    iot-protocols/amqp/classes/wrappers/messageID/amqplongid.cpp \
    iot-protocols/amqp/classes/wrappers/messageID/amqpstringid.cpp \
    iot-protocols/amqp/classes/wrappers/messageID/amqpuuid.cpp \
    iot-protocols/amqp/classes/wrappers/amqpdecimal.cpp \
    iot-protocols/amqp/classes/wrappers/amqpmessageformat.cpp \
    iot-protocols/amqp/classes/wrappers/amqpsymbol.cpp \
    iot-protocols/amqp/classes/headerapi/amqpwrapper.cpp \
    iot-protocols/amqp/classes/headerapi/amqpunwrapper.cpp \
    iot-protocols/amqp/classes/tlv/described/amqperror.cpp \
    iot-protocols/amqp/classes/tlv/described/amqprejected.cpp \
    iot-protocols/amqp/classes/tlv/described/amqpreleased.cpp \
    iot-protocols/amqp/classes/tlv/described/amqpmodified.cpp \
    iot-protocols/amqp/classes/tlv/api/amqptlvfactory.cpp \
    iot-protocols/amqp/classes/headerapi/amqpheader.cpp \
    iot-protocols/amqp/classes/headerapi/amqpping.cpp \
    iot-protocols/amqp/classes/headerapi/amqpprotoheader.cpp \
    iot-protocols/amqp/classes/amqptransfermap.cpp \
    iot-protocols/amqp/classes/terminus/amqpsource.cpp \
    iot-protocols/amqp/classes/terminus/amqptarget.cpp \
    iot-protocols/amqp/classes/headerapi/amqpfactory.cpp \
    iot-protocols/amqp/classes/headeramqp/amqpattach.cpp \
    iot-protocols/amqp/classes/headeramqp/amqpclose.cpp \
    iot-protocols/amqp/classes/headeramqp/amqpend.cpp \
    iot-protocols/amqp/classes/headeramqp/amqpdetach.cpp \
    iot-protocols/amqp/classes/headeramqp/amqpdisposition.cpp \
    iot-protocols/amqp/classes/headeramqp/amqptransfer.cpp \
    iot-protocols/amqp/classes/section/amqpdata.cpp \
    iot-protocols/amqp/classes/section/amqpfooter.cpp \
    iot-protocols/amqp/classes/section/amqpsequence.cpp \
    iot-protocols/amqp/classes/section/amqpvalue.cpp \
    iot-protocols/amqp/classes/section/amqpapplicationproperties.cpp \
    iot-protocols/amqp/classes/section/amqpproperties.cpp \
    iot-protocols/amqp/classes/section/amqpdeliveryannotation.cpp \
    iot-protocols/amqp/classes/section/amqpmessageannotations.cpp \
    iot-protocols/amqp/classes/section/amqpmessageheader.cpp \
    iot-protocols/amqp/classes/headeramqp/amqpbegin.cpp \
    iot-protocols/amqp/classes/headeramqp/amqpflow.cpp \
    iot-protocols/amqp/classes/headeramqp/amqpopen.cpp \
    iot-protocols/amqp/classes/headersasl/amqpsaslchallenge.cpp \
    iot-protocols/amqp/classes/headersasl/amqpsaslinit.cpp \
    iot-protocols/amqp/classes/headersasl/amqpsaslmechanisms.cpp \
    iot-protocols/amqp/classes/headersasl/amqpsasloutcome.cpp \
    iot-protocols/amqp/classes/headersasl/amqpsaslresponse.cpp \
    iot-protocols/amqp/classes/parser/amqpparser.cpp \
    iot-protocols/amqp/classes/amqp.cpp \
    iot-protocols/classes/iotenumprotocol.cpp \
    loadingform.cpp \
    generalform.cpp \
    accountlistform.cpp \
    loginform.cpp

HEADERS  += mainwindow.h \
    database/entities/accountentity.h \
    database/entities/topicentity.h \
    database/database.h \
    database/entities/messageentity.h \
    database/accountmanager.h \
    listwidget.h \
    cells/cellwithcheckbox.h \
    cells/cellwitheditline.h \
    cells/cellwithcombobox.h \
    tabs/topiclisttab.h \
    cells/celltopicitem.h \
    tabs/sendmessagetab.h \
    tabs/messageslisttab.h \
    cells/cellmessageitem.h \
    cells/cellaccountitem.h \
    timer/timertask.h \
    timer/timersmap.h \
    internet-protocols/internetprotocol.h \
    internet-protocols/tcpsocket.h \
    internet-protocols/udpsocket.h \
    iot-protocols/iotprotocol.h \
    iot-protocols/mqtt/mqtt.h \
    classes/bytearray.h \
    classes/lengthdetails.h \
    iot-protocols/classes/countablemessage.h \
    iot-protocols/classes/message.h \
    iot-protocols/classes/qos.h \
    iot-protocols/mqtt/classes/will.h \
    iot-protocols/mqtt/messages/connack.h \
    iot-protocols/mqtt/messages/connect.h \
    iot-protocols/mqtt/messages/disconnect.h \
    iot-protocols/mqtt/messages/pingreq.h \
    iot-protocols/mqtt/messages/pingresp.h \
    iot-protocols/mqtt/messages/puback.h \
    iot-protocols/mqtt/messages/pubcomp.h \
    iot-protocols/mqtt/messages/publish.h \
    iot-protocols/mqtt/messages/pubrec.h \
    iot-protocols/mqtt/messages/pubrel.h \
    iot-protocols/mqtt/messages/suback.h \
    iot-protocols/mqtt/messages/subscribe.h \
    iot-protocols/mqtt/messages/unsuback.h \
    iot-protocols/mqtt/messages/unsubscribe.h \
    iot-protocols/mqtt/parser/messagesparser.h \
    iot-protocols/mqtt/parser/parser.h \
    iot-protocols/protocolmessage.h \
    iot-protocols/mqtt-sn/mqttsn.h \
    iot-protocols/mqtt-sn/messages/snadvertise.h \
    iot-protocols/mqtt-sn/messages/snencapsulated.h \
    iot-protocols/mqtt-sn/messages/sngwinfo.h \
    iot-protocols/mqtt-sn/messages/snregack.h \
    iot-protocols/mqtt-sn/messages/snregister.h \
    iot-protocols/mqtt-sn/messages/snconnack.h \
    iot-protocols/mqtt-sn/messages/snconnect.h \
    iot-protocols/mqtt-sn/messages/sndisconnect.h \
    iot-protocols/mqtt-sn/messages/snpingreq.h \
    iot-protocols/mqtt-sn/messages/snpingresp.h \
    iot-protocols/mqtt-sn/messages/snpuback.h \
    iot-protocols/mqtt-sn/messages/snpubcomp.h \
    iot-protocols/mqtt-sn/messages/snpublish.h \
    iot-protocols/mqtt-sn/messages/snpubrec.h \
    iot-protocols/mqtt-sn/messages/snpubrel.h \
    iot-protocols/mqtt-sn/messages/snsuback.h \
    iot-protocols/mqtt-sn/messages/snsubscribe.h \
    iot-protocols/mqtt-sn/messages/snunsuback.h \
    iot-protocols/mqtt-sn/messages/snunsubscribe.h \
    iot-protocols/mqtt-sn/messages/snsearchgw.h \
    iot-protocols/mqtt-sn/messages/snwillmsg.h \
    iot-protocols/mqtt-sn/messages/snwillmsgreq.h \
    iot-protocols/mqtt-sn/messages/snwillmsgresp.h \
    iot-protocols/mqtt-sn/messages/snwillmsgupd.h \
    iot-protocols/mqtt-sn/messages/snwilltopic.h \
    iot-protocols/mqtt-sn/messages/snwilltopicreq.h \
    iot-protocols/mqtt-sn/messages/snwilltopicresp.h \
    iot-protocols/mqtt-sn/messages/snwilltopicupd.h \
    iot-protocols/mqtt/classes/mqttenums.h \
    iot-protocols/mqtt/classes/mqtopic.h \
    iot-protocols/classes/topic.h \
    iot-protocols/mqtt-sn/classes/mqttsnenums.h \
    iot-protocols/mqtt-sn/classes/topics/snfulltopic.h \
    iot-protocols/mqtt-sn/classes/topics/snidentifiertopic.h \
    iot-protocols/mqtt-sn/classes/topics/snshorttopic.h \
    iot-protocols/mqtt-sn/classes/snresponsemessage.h \
    iot-protocols/mqtt-sn/classes/snvaluesvalidator.h \
    iot-protocols/mqtt-sn/classes/sncontrols.h \
    iot-protocols/mqtt-sn/classes/snflags.h \
    iot-protocols/mqtt-sn/parser/snparser.h \
    iot-protocols/mqtt-sn/parser/snmessagesparser.h \
    iot-protocols/coap/coap.h \
    iot-protocols/coap/classes/coapenums.h \
    iot-protocols/coap/messages/coapmessage.h \
    iot-protocols/coap/parser/coapparser.h \
    iot-protocols/amqp/classes/enums/amqpdistributionmode.h \
    iot-protocols/amqp/classes/enums/enumobject.h \
    iot-protocols/amqp/classes/enums/amqperrorcode.h \
    iot-protocols/amqp/classes/enums/amqpheadercode.h \
    iot-protocols/amqp/classes/enums/amqplifetimepolicy.h \
    iot-protocols/amqp/classes/enums/amqpreceiversettlemode.h \
    iot-protocols/amqp/classes/enums/amqprolecode.h \
    iot-protocols/amqp/classes/enums/amqpsaslcode.h \
    iot-protocols/amqp/classes/enums/amqpsectioncode.h \
    iot-protocols/amqp/classes/enums/amqpsendcode.h \
    iot-protocols/amqp/classes/enums/amqpstatecode.h \
    iot-protocols/amqp/classes/enums/amqpterminusdurability.h \
    iot-protocols/amqp/classes/enums/amqpterminusexpirypolicy.h \
    iot-protocols/amqp/classes/enums/amqptype.h \
    iot-protocols/amqp/classes/enums/amqpconnectionproperty.h \
    iot-protocols/amqp/classes/enums/amqpdynamicnodeproperty.h \
    iot-protocols/amqp/classes/constructors/amqpsimpleconstructor.h \
    iot-protocols/amqp/classes/constructors/amqpdescribedconstructor.h \
    iot-protocols/amqp/classes/tlv/api/tlvamqp.h \
    iot-protocols/amqp/classes/tlv/variable/amqptlvvariable.h \
    iot-protocols/amqp/classes/tlv/fixed/amqptlvnull.h \
    iot-protocols/amqp/classes/tlv/fixed/amqptlvfixed.h \
    iot-protocols/amqp/classes/tlv/array/amqptlvarray.h \
    iot-protocols/amqp/classes/tlv/compound/amqptlvmap.h \
    iot-protocols/amqp/classes/tlv/compound/amqptlvlist.h \
    iot-protocols/amqp/classes/tlv/described/amqpoutcome.h \
    iot-protocols/amqp/classes/tlv/described/amqpstate.h \
    iot-protocols/amqp/classes/tlv/described/amqpaccepted.h \
    iot-protocols/amqp/classes/tlv/described/amqplifetimepolicyobject.h \
    iot-protocols/amqp/classes/tlv/described/amqpreceived.h \
    iot-protocols/amqp/classes/amqpsimpletype.h \
    iot-protocols/amqp/classes/wrappers/messageID/amqpmessageid.h \
    iot-protocols/amqp/classes/wrappers/messageID/amqpbinaryid.h \
    iot-protocols/amqp/classes/wrappers/messageID/amqplongid.h \
    iot-protocols/amqp/classes/wrappers/messageID/amqpstringid.h \
    iot-protocols/amqp/classes/wrappers/messageID/amqpuuid.h \
    iot-protocols/amqp/classes/wrappers/amqpdecimal.h \
    iot-protocols/amqp/classes/wrappers/amqpmessageformat.h \
    iot-protocols/amqp/classes/wrappers/amqpsymbol.h \
    iot-protocols/amqp/classes/headerapi/amqpwrapper.h \
    iot-protocols/amqp/classes/headerapi/amqpunwrapper.h \
    iot-protocols/amqp/classes/tlv/described/amqperror.h \
    iot-protocols/amqp/classes/tlv/described/amqprejected.h \
    iot-protocols/amqp/classes/tlv/described/amqpreleased.h \
    iot-protocols/amqp/classes/tlv/described/amqpmodified.h \
    iot-protocols/amqp/classes/tlv/api/amqptlvfactory.h \
    iot-protocols/amqp/classes/headerapi/amqpheader.h \
    iot-protocols/amqp/classes/headerapi/amqpping.h \
    iot-protocols/amqp/classes/headerapi/amqpprotoheader.h \
    iot-protocols/amqp/classes/amqptransfermap.h \
    iot-protocols/amqp/classes/terminus/amqpsource.h \
    iot-protocols/amqp/classes/terminus/amqptarget.h \
    iot-protocols/amqp/classes/headerapi/amqpfactory.h \
    iot-protocols/amqp/classes/amqpsection.h \
    iot-protocols/amqp/classes/section/amqpsection.h \
    iot-protocols/amqp/classes/headeramqp/amqpattach.h \
    iot-protocols/amqp/classes/headeramqp/amqpclose.h \
    iot-protocols/amqp/classes/headeramqp/amqpend.h \
    iot-protocols/amqp/classes/headeramqp/amqpdetach.h \
    iot-protocols/amqp/classes/headeramqp/amqpdisposition.h \
    iot-protocols/amqp/classes/headeramqp/amqptransfer.h \
    iot-protocols/amqp/classes/section/amqpdata.h \
    iot-protocols/amqp/classes/section/amqpfooter.h \
    iot-protocols/amqp/classes/section/amqpsequence.h \
    iot-protocols/amqp/classes/section/amqpvalue.h \
    iot-protocols/amqp/classes/section/amqpapplicationproperties.h \
    iot-protocols/amqp/classes/section/amqpproperties.h \
    iot-protocols/amqp/classes/section/amqpdeliveryannotation.h \
    iot-protocols/amqp/classes/section/amqpmessageannotations.h \
    iot-protocols/amqp/classes/section/amqpmessageheader.h \
    iot-protocols/amqp/classes/headeramqp/amqpbegin.h \
    iot-protocols/amqp/classes/headeramqp/amqpflow.h \
    iot-protocols/amqp/classes/headeramqp/amqpopen.h \
    iot-protocols/amqp/classes/headersasl/amqpsaslchallenge.h \
    iot-protocols/amqp/classes/headersasl/amqpsaslinit.h \
    iot-protocols/amqp/classes/headersasl/amqpsaslmechanisms.h \
    iot-protocols/amqp/classes/headersasl/amqpsasloutcome.h \
    iot-protocols/amqp/classes/headersasl/amqpsaslresponse.h \
    iot-protocols/amqp/classes/parser/amqpparser.h \
    iot-protocols/amqp/classes/amqp.h \
    iot-protocols/classes/iotenumprotocol.h \
    loadingform.h \
    generalform.h \
    accountlistform.h \
    loginform.h

FORMS    += mainwindow.ui \
    cells/cellwithcheckbox.ui \
    cells/cellwitheditline.ui \
    cells/cellwithcombobox.ui \
    tabs/topiclisttab.ui \
    cells/celltopicitem.ui \
    tabs/sendmessagetab.ui \
    tabs/messageslisttab.ui \
    cells/cellmessageitem.ui \
    cells/cellaccountitem.ui \
    loadingform.ui \
    generalform.ui \
    accountlistform.ui \
    loginform.ui

RESOURCES += \
    resources.qrc
