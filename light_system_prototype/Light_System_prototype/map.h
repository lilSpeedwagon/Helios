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
#include "point.h"
#include "camera.h"

/* Класс, описывающий карту, на которой расположены устройства освещения Device (и замки), а так же посетители и сотрудники, описываемые классом Person. Map при инстанциации получает карту
Умного квартала из файла и, затем сохраняет её конфигурацию для отображения в GUI и хранит положение устройств освещения в lightDevices. */

class Map : public QObject
{
    Q_OBJECT
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
    Client *client;
    Devices *devices;
    QMap<QString, Camera> *cameras;

public:
    static Map& getMap();
    static Map* getMapPtr();
    float getWidth() const;
    float getHeight() const;
    void setWidth(float width);
    void setHeight(float height);
    void checkPositions(bool value);
    void setSender(Client *client);
    void setDevices(Devices *devices);

    bool readMap(QString fileName);

public slots:
    void slotProcessData(QString data);
};

#endif // MAP_H
