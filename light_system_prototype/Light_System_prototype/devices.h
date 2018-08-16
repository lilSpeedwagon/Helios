#ifndef DEVICES_H
#define DEVICES_H

#include <QMap>
#include "device.h"

/* Singleton класс для хранения массива устройств (существующих по факту) и обращения к ним из любого участка программы. */

class Devices
{
private:
    static QMap<QString, Device> *devices;
    // Конструкторы и оператор присваивания недоступны клиентам
    Devices();
    Devices( const Devices& );
    Devices& operator=( Devices& );
public:
    static QMap<QString, Device> * getDevices();
};

#endif // DEVICES_H
