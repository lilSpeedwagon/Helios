#include "device.h"

Device::Device()    {}

Device::Device(QString name, Point position) : name(name), position(position) {}

Device::Device(QString name, QString adress) : name(name), adress(adress)   {}

Device::Device(QString name, QString adress, Point position) : name(name), adress(adress), position(position)   {}

int Device::getId() const   {
    return id;
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

QString Device::getPowerStateStr() const    {
    if (powerState == PowerState::ENABLED)
        return "Enabled";
    else
        return "Disabled";
}

Point Device::getPosition() const   {
    return position;
}

QString Device::getNetworkState() const {
    if (isConnected())  {
        return "Connected";
    }
    return "Disconnected";
}

bool Device::isEnabled() const  {
    return (powerState == PowerState::ENABLED);
}

bool Device::isConnected() const    {
    return !adress.isEmpty();
}

void Device::setId(int id)  {
    this->id = id;
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

void Device::changeState()  {
    if (powerState == PowerState::ENABLED)  {
        powerState = PowerState::DISABLED;
    }   else    {
        powerState = PowerState::ENABLED;
    }
}

Device::~Device()   {

}
