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
    parser/parser.cpp \
    parser/supporting/lengthdetails.cpp \
    parser/supporting/qos.cpp \
    parser/supporting/topic.cpp \
    parser/supporting/will.cpp \
    parser/messages/connack.cpp \
    parser/messages/connect.cpp \
    parser/messages/disconnect.cpp \
    parser/messages/pingreq.cpp \
    parser/messages/pingresp.cpp \
    parser/messages/puback.cpp \
    parser/messages/pubcomp.cpp \
    parser/messages/publish.cpp \
    parser/messages/pubrec.cpp \
    parser/messages/pubrel.cpp \
    parser/messages/suback.cpp \
    parser/messages/subscribe.cpp \
    parser/messages/unsuback.cpp \
    parser/messages/unsubscribe.cpp \
    parser/supporting/bytearray.cpp \
    mqtt/socket/clientsocket.cpp \
    mqtt/mqtt.cpp \
    parser/messagesparser.cpp \
    database/database.cpp \
    database/accountmanager.cpp \
    logindialog.cpp \
    listwidget.cpp \
    cells/cellwithcheckbox.cpp \
    cells/cellwitheditline.cpp \
    cells/cellwithcombobox.cpp \
    tabs/topiclisttab.cpp \
    cells/celltopicitem.cpp \
    tabs/sendmessagetab.cpp \
    tabs/messageslisttab.cpp \
    cells/cellmessageitem.cpp \
    loadingdialog.cpp \
    accountslistdialog.cpp \
    cells/cellaccountitem.cpp \
    mqtt/mqttmanager.cpp \
    timer/timertask.cpp \
    timer/timersmap.cpp

HEADERS  += mainwindow.h \
    parser/parser.h \
    parser/messages/message.h \
    parser/supporting/lengthdetails.h \
    parser/supporting/qos.h \
    parser/supporting/topic.h \
    parser/supporting/will.h \
    parser/messages/connack.h \
    parser/supporting/protocolmessage.h \
    parser/messages/connect.h \
    parser/messages/disconnect.h \
    parser/messages/pingreq.h \
    parser/messages/pingresp.h \
    parser/messages/puback.h \
    parser/messages/pubcomp.h \
    parser/messages/publish.h \
    parser/messages/pubrec.h \
    parser/messages/pubrel.h \
    parser/messages/suback.h \
    parser/messages/subscribe.h \
    parser/messages/unsuback.h \
    parser/messages/unsubscribe.h \
    parser/supporting/bytearray.h \
    mqtt/socket/clientsocket.h \
    mqtt/mqtt.h \
    parser/messagesparser.h \
    database/entities/accountentity.h \
    database/entities/topicentity.h \
    database/database.h \
    database/entities/messageentity.h \
    database/accountmanager.h \
    logindialog.h \
    listwidget.h \
    cells/cellwithcheckbox.h \
    cells/cellwitheditline.h \
    cells/cellwithcombobox.h \
    tabs/topiclisttab.h \
    cells/celltopicitem.h \
    tabs/sendmessagetab.h \
    tabs/messageslisttab.h \
    cells/cellmessageitem.h \
    loadingdialog.h \
    accountslistdialog.h \
    cells/cellaccountitem.h \
    mqtt/mqttmanager.h \
    timer/timertask.h \
    timer/timersmap.h \
    parser/messages/countablemessage.h

FORMS    += mainwindow.ui \
    logindialog.ui \
    cells/cellwithcheckbox.ui \
    cells/cellwitheditline.ui \
    cells/cellwithcombobox.ui \
    tabs/topiclisttab.ui \
    cells/celltopicitem.ui \
    tabs/sendmessagetab.ui \
    tabs/messageslisttab.ui \
    cells/cellmessageitem.ui \
    loadingdialog.ui \
    accountslistdialog.ui \
    cells/cellaccountitem.ui

RESOURCES += \
    resources.qrc
