#include "devices.h"

//статический член класса, согласно стандарту, должен быть определен где то вне класса
QMap<QString, Device>* Devices::devices;

QMap<QString, Device>* Devices::getDevices()    {
    if(!devices)    {
        qDebug() << "-----------------------------";
        qDebug() << "devices initializing...";
        devices = new QMap<QString, Device>();
        qDebug() << "Done";
    }
    return devices;
}

void Devices::add(QString name, Device device)  {
    devices->insert(name, device);
    emit SIGNAL(signalNewDevice());
}

void Devices::remove(QString name)   {
    devices->remove(name);
    emit SIGNAL(signalRemoveDevice());
}
