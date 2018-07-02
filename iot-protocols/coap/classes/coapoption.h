#ifndef COAPOPTION_H
#define COAPOPTION_H

#include <QByteArray>
#include <QObject>

class CoapOption
{
private:
    int number;
    int length;
    QByteArray value;
public:
    CoapOption();
    CoapOption(int number, int length, QByteArray value);

    static bool numberLessThan(const CoapOption &op1, const CoapOption &op2);

    int getNumber() const;
    void setNumber(int value);
    int getLength() const;
    void setLength(int value);
    void setValue(const QByteArray &value);
    QByteArray getValue() const;
};

#endif // COAPOPTION_H
