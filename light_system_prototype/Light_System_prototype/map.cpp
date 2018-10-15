#include "map.h"

//статический член класса, согласно стандарту, должен быть определен где то вне класса
Map* Map::map;
Client* Map::client;
Devices* Map::devices;

Map::Map()
{
    qDebug() << "-----------------------------";
    qDebug() << "Map init...";
    readMap("C:\\docs\\programms\\Future Gadgets LAb\\Smart_district\\Helios\\light_system_prototype\\smartDistrict.lmap");
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

bool Map::readMap(QString fileName) {
    bool result = false;

    mapFile = new QFile(fileName);
    if (mapFile->exists() && mapFile->open(QIODevice::ReadOnly))   {
        qDebug() << "map file has opened.";
        if (devices != nullptr)
            devices->clear();
        else
            qWarning() << "No device list for map reader";
        qDebug() << "Reading...";
        if (mapFile->readLine().startsWith("<map>"))    {
            while (!mapFile->atEnd())    {
                QString tempStr = mapFile->readLine();
                if (tempStr.startsWith("<light") /* && tempStr.endsWith("</light>") */ )   {       //для строка типа <light name='name' x='15' y='20'>
                    QString tempName = tempStr.section("\'", 1, 1);
                    float x = tempStr.section("\'", 3, 3).toFloat();
                    float y = tempStr.section("\'", 5, 5).toFloat();
                    qDebug() << "Light object with name = " << tempName << ", x = " << x << ", y = " << y << " found";

                    devices->add(tempName, Device(tempName, Point(x, y)));
                }
                //здесь должно быть так же определение lock и геометрии карты
            }
            result = true;
        }   else    {
            qCritical() << "Incorrect map file :(";            
        }

        mapFile->close();

        qDebug() << "Done. Map file has been successfuly red.";
    }   else    {
        qCritical() << "map file not found or it's incorrect :(";
    }
    return result;
}

const float Map::TRIGGER_DISTANTION = 100;

void Map::checkPositions() {
    qDebug() << "checking positions of all persons...";
    if (client != nullptr)   {
        for (auto &device : devices->getDevices())  {
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
    }   else    {
        qDebug() << "No sender for this map.";
    }

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

void Map::setSender(Client *newClient) {
    client = newClient;
}

void Map::setDevices(Devices *newDevices)  {
    devices = newDevices;
}
