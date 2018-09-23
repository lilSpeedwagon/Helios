#ifndef MAP_H
#define MAP_H

#include <QDebug>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QTimer>
//#include "tcp.h"
#include "client.h"
#include "device.h"
#include "devices.h"
#include "person.h"
#include "persons.h"
#include "point.h"

/* Класс, описывающий карту, на которой расположены устройства освещения Device (и замки), а так же посетители и сотрудники, описываемые классом Person. Map при инстанциации получает карту
Умного квартала из файла и, затем сохраняет её конфигурацию для отображения в GUI и хранит положение устройств освещения в lightDevices. */

class Map : QObject
{
private:
    static Map *map;
    Map();
    Map(Map const&);
    void operator=(Map const&);
    float width;
    float height;
    QFile *mapFile;
    void readMap();
    void checkPositions();
    QTimer *timer;
    void initTimer();
    float  frequency = 1;
    static const float triggerDistation;    //дистанция реакции устройств
public:
    static Map& getMap();
    float getWidth() const;
    float getHeight() const;
    void setWidth(float width);
    void setHeight(float height);
    void checkPositions(bool value);

public slots:
    void slotCheckPositions();
};

#endif // MAP_H
