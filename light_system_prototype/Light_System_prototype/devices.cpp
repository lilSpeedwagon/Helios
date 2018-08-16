#include "devices.h"

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
