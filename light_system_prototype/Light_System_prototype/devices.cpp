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

void Devices::add(QString name, Device const& device)  {
    devices->insert(name, device);
    emit signalNewDevice();
}

void Devices::remove(QString name)   {
    devices->remove(name);
    emit signalRemoveDevice();
}

void Devices::clear()   {
    qDebug() << "Clearing device list...";
    devices->clear();
}

void Devices::clearAdresses()   {
    for (Device &device : *devices)   {
        if (!device.getAdress().isEmpty())
            device.setAdress(0);
    }
}
QMap<QString, Device>& Devices::getDevices() const  {
    return *devices;
}

int Devices::size() const   {
    return devices->size();
}

void Devices::signalNewDevice() {}
void Devices::signalRemoveDevice() {};
