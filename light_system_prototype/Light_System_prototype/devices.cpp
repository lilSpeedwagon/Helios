#include "devices.h"

Devices::Devices()  {
    devices = new QMap<QString, Device>();
}

Devices::Devices(Devices const& d)  {
    devices = new QMap<QString, Device>(*d.devices);
}

Devices& Devices::operator=(Devices const& d)   {
    if (devices != nullptr)
        delete devices;
    devices = new QMap<QString, Device>(*d.devices);
    return *this;
}

void Devices::add(QString name, Device device)  {
    devices->insert(name, device);
    emit SIGNAL(signalNewDevice());
}

void Devices::remove(QString name)   {
    devices->remove(name);
    emit SIGNAL(signalRemoveDevice());
}

void Devices::clear()   {
    devices->clear();
}

QMap<QString, Device>& Devices::getDevices() const  {
    return *devices;
}
