#include "device.h"

const QString Device::MESSAGE_ON = "#ON#";
const QString Device::MESSAGE_OFF = "#OFF#";
const QString Device::MESSAGE_ASK = "#HOW#";

Device::Device()    {}

Device::Device(QString name, Point position) : name(name), position(position) {}

Device::Device(QTcpSocket* socket) : socket(socket) {}

Device::Device(QTcpSocket* socket, QString name) : name(name), socket(socket) {}

Device::Device(QTcpSocket* socket, QString name, PowerState powerState) :
    name(name), powerState(powerState), socket(socket) {}

Device::Device(QTcpSocket* socket, QString name, int powerState) : name(name), socket(socket)  {
    if (powerState)
        powerState = PowerState::ENABLED;
    else
        powerState = PowerState::DISABLED;
}

QString Device::getName() const   {
    return name;
}

QString Device::getAdress() const {
    return adress;
}

PowerState Device::getPowerState() const  {
    return powerState;
}

QTcpSocket* Device::getSocket() const {
    return socket;
}

Point Device::getPosition() const   {
    return position;
}

void Device::setName(QString name)  {
    this->name = name;
}

void Device::setAdress(QString adress)  {
    this->adress = adress;
}

void Device::setPowerState(PowerState powerState)  {
    this->powerState = powerState;
}

void Device::setPowerState(int powerState)  {
    if (powerState)
        this->powerState = PowerState::ENABLED;
    else
        this->powerState = PowerState::DISABLED;
}

void Device::setSocket(QTcpSocket* socket)  {
    this->socket = socket;
}

void Device::setPosition(Point position)    {
    this->position = position;
}
