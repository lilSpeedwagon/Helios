#ifndef MAP_H
#define MAP_H

#include <QDebug>
#include <QMap>
#include <QDebug>
#include "device.h"
#include "devices.h"
#include "point.h"

/* Класс, описывающий карту, на которой расположены устройства освещения Device (и замки), а так же посетители и сотрудники, описываемые классом Person. Map при инстанциации получает карту
Умного квартала из файла и, затем сохраняет её конфигурацию для отображения в GUI и хранит положение устройств освещения в lightDevices. */

class Map
{
private:
    static Map *map;
    Map();
    Map(Map const&);
    void operator=(Map const&);
    float width;
    float height;
    void readMap();
public:
    static Map& getMap();
    float getWidth() const;
    float getHeight() const;
    void setWidth(float width);
    void setHeight(float height);
};

#endif // MAP_H
