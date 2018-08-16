#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QTcpSocket>
#include "point.h"

enum PowerState { ENABLED, DISABLED };

class Device
{
private:
    QString name;
    QString adress;
    PowerState powerState = PowerState::DISABLED;
    QTcpSocket* socket;

    Point position;

public:
    static const QString MESSAGE_ON;
    static const QString MESSAGE_OFF;
    static const QString MESSAGE_ASK;

    Device();
    Device(QString name, Point position);
    Device(QTcpSocket *socket);
    Device(QTcpSocket *socket, QString name);
    Device(QTcpSocket* socket, QString name, PowerState powerState);
    Device(QTcpSocket* socket, QString name, int powerState);
    QString getName() const;
    QString getAdress() const;
    PowerState getPowerState() const;
    QTcpSocket* getSocket() const;
    Point getPosition() const;

    void setName(QString name);
    void setAdress(QString adress);
    void setPowerState(PowerState powerState);
    void setPowerState(int powerState);
    void setSocket(QTcpSocket *socket);
    void setPosition(Point position);
};

#endif // DEVICE_H
