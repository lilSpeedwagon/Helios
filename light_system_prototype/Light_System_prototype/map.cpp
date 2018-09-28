#include "map.h"

//статический член класса, согласно стандарту, должен быть определен где то вне класса
Map* Map::map;

Map::Map()
{
    qDebug() << "-----------------------------";
    qDebug() << "Map init...";
    readMap();
}

Map& Map::getMap()  {
    if (!map)
        map = new Map();
    return *map;
}

float Map::getWidth() const {
    return width;
}

float Map::getHeight() const {
    return height;
}

void Map::setWidth(float width) {
    this->width = width;
}

void Map::setHeight(float height)   {
    this->height = height;
}

void Map::readMap() {
    mapFile = new QFile("C:\\docs\\programms\\Future Gadgets LAb\\Smart_district\\Helios\\light_system_prototype\\smartDistrict.lmap");
    if (mapFile->exists() && mapFile->open(QIODevice::ReadOnly))   {
        qDebug() << "map file has opened.";
        qDebug() << "Reading...";
        while (!mapFile->atEnd())    {
            QString tempStr = mapFile->readLine();
            if (tempStr.startsWith("<light") /* && tempStr.endsWith("</light>") */ )   {       //для строка типа <light name='name' x='15' y='20'></light>
                QString tempName = tempStr.section("\'", 1, 1);
                float x = tempStr.section("\'", 3, 3).toFloat();
                float y = tempStr.section("\'", 5, 5).toFloat();
                qDebug() << "Light object with name = " << tempName << ", x = " << x << ", y = " << y << " found";
                Device tempDevice(tempName, Point(x, y));
                Devices::add(tempName, tempDevice);
            }
            //здесь должно быть так же определение lock и геометрии карты
        }
        mapFile->close();
        qDebug() << "Done. Map file has been successfuly red.";
    }   else    {
        qCritical() << "map file not found or it's incorrect :(";
        qCritical() << "Aborting programm.";
        exit(1);
    }
}

const float Map::TRIGGER_DISTANTION = 100;

void Map::checkPositions() {
    qDebug() << "checking positions of all persons...";
    for (auto &device : *Devices::getDevices())  {
        for (auto &person : *Persons::getPersons()) {
            if (Point::distantion(device.getPosition(), person.getPosition()) <= TRIGGER_DISTANTION)    {
                if (device.getPowerState() == PowerState::DISABLED) {
                    qDebug() << person.getName() << " is entering to device " << device.getName() << " zone. Enabling device...";
                    //TCP::getTCP()->sendToClient(device, Device::MESSAGE_ON);
                    client->sendToDevice(device, Client::MESSAGE_ON);
                }
            }   else    {
                if (device.getPowerState() == PowerState::ENABLED)  {
                    qDebug() << person.getName() << " is leaving device " << device.getName() << " zone. Disabling device...";
                    //TCP::getTCP()->sendToClient(device, Client::MESSAGE_OFF);
                    client->sendToDevice(device, Client::MESSAGE_OFF);
                }
            }
        }
    }
    qDebug() << "Done.";
}

void Map::initTimer()   {
    qDebug() << "initializing map timer...";
    timer = new QTimer();
    timer->setInterval((int) 1000 / frequency);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotCheckPositions));
    qDebug() << "Done";
}

void Map::destroyTimer()    {
    qDebug() << "destroying map timer...";

    timer->stop();
    delete timer;

    qDebug() << "Done.";
}

void Map::checkPositions(bool value)    {
    if (timer == nullptr)  {
        initTimer();
    }
    if (value)  {
        qDebug() << "starting checking map every " << int(1000 / frequency) << " seconds...";
        timer->start();
    }   else    {
        qDebug() << "Stopping checking map...";
        timer->stop();
    }
}

void Map::slotCheckPositions()  {
    checkPositions();
}

void Map::setSender(Client *client) {
    this->client = client;
}
