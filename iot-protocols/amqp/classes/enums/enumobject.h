#ifndef ENUMOBJECT_H
#define ENUMOBJECT_H

#include <QObject>

class EnumObject : public QObject
{
    Q_OBJECT
public:
    explicit EnumObject(QObject *parent = 0);

signals:

public slots:
};

#endif // ENUMOBJECT_H