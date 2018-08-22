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
    Devices(Devices const& );
    Devices& operator=(Devices const&);
public:
    static QMap<QString, Device> * getDevices();
    static void add(QString name, Device device);
    static void remove(QString name);
};

/* отдельные статические методы по сути просто являются оболочкой над обычными insert и remove для коллекций, но дополнительно вырабатывают сигналы для работы с GUI */

#endif // DEVICES_H
