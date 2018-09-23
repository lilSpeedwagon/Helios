#include "device.h"

Device::Device()    {}

Device::Device(QString name, Point position) : name(name), position(position) {}

Device::Device(QString name, QString adress) : name(name), adress(adress)   {}

Device::Device(QString name, QString adress, Point position) : name(name), adress(adress), position(position)   {}

QString Device::getName() const   {
    return name;
}

QString Device::getAdress() const {
    return adress;
}

PowerState Device::getPowerState() const  {
    return powerState;
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

void Device::setPosition(Point position)    {
    this->position = position;
}
