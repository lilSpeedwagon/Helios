#include "devices.h"

//статический член класса, согласно стандарту, должен быть определен где то вне класса
QMap<QString, Device>* Devices::devices;

Devices::Devices()
{

}

QMap<QString, Device>* Devices::getDevices()    {
    if(!devices)    {
        qDebug() << "devices initializing...";
        devices = new QMap<QString, Device>();
        qDebug() << "Done";
    }
    return devices;
}
