#ifndef IOTPROTOCOL_H
#define IOTPROTOCOL_H

#include <QObject>

class IotProtocol : public QObject
{
    Q_OBJECT
public:
    explicit IotProtocol(QObject *parent = 0);

signals:

public slots:
};

#endif // IOTPROTOCOL_H