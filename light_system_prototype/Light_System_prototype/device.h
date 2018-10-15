#ifndef DEVICE_H
#define DEVICE_H

#include <QString>
#include <QTcpSocket>
#include "point.h"

enum PowerState { ENABLED, DISABLED };

class Device
{
private:
    int id = 0;
    QString name;
    QString adress;
    PowerState powerState = PowerState::DISABLED;

    Point position = Point(0, 0);

public:
    Device();
    Device(QString name, Point position);
    Device(QString name, QString adress);
    Device(QString name, QString adress, Point position);
    ~Device();
    int getId() const;
    QString getName() const;
    QString getAdress() const;
    PowerState getPowerState() const;
    QString getPowerStateStr() const;
    Point getPosition() const;

    bool isEnabled() const;
    bool isConnected() const;

    void setId(int id);
    void setName(QString name);
    void setAdress(QString adress);
    void setPowerState(PowerState powerState);
    void setPowerState(int powerState);
    void setPosition(Point position);

    void changeState();
};

#endif // DEVICE_H
