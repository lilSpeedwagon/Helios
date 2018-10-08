#ifndef MAP_H
#define MAP_H

#include <QDebug>
#include <QMap>
#include <QDebug>
#include <QFile>
#include <QTimer>
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
    float width;
    float height;
    float  frequency = 1;
    static const float TRIGGER_DISTANTION;    //дистанция реакции устройств

    static Map *map;
    Map();
    Map(Map const&);
    void operator=(Map const&);

    QFile *mapFile;
    QTimer *timer;
    static Client *client;
    static Devices *devices;

    void initTimer();
    void destroyTimer();
    void checkPositions();

public:
    static Map& getMap();
    float getWidth() const;
    float getHeight() const;
    void setWidth(float width);
    void setHeight(float height);
    void checkPositions(bool value);
    static void setSender(Client *client);
    static void setDevices(Devices *devices);

    void readMap(QString fileName);

public slots:
    void slotCheckPositions();
};

#endif // MAP_H
