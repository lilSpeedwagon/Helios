#include "map.h"

//статический член класса, согласно стандарту, должен быть определен где то вне класса
Map* Map::map;
Client* Map::client;
Devices* Map::devices;
QMap<QString, Camera>* Map::cameras;

Map::Map()
{
    qDebug() << "-----------------------------";
    qDebug() << "Map init...";
    cameras = new QMap<QString, Camera>();
    readMap("C:\\docs\\programms\\Future Gadgets LAb\\Smart_district\\Helios\\light_system_prototype\\smartDistrict.lmap");
}

Map& Map::getMap()  {
    if (!map)
        map = new Map();
    return *map;
}

Map* Map::getMapPtr()  {
    if (!map)
        map = new Map();
    return map;
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

        if (cameras != nullptr)
            cameras->clear();
        else
            qWarning() << "No cameras list for map reader";

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
                if (tempStr.startsWith("<camera"))   {
                    QString tempName = tempStr.section("\'", 1, 1);
                    float x = tempStr.section("\'", 3, 3).toFloat();
                    float y = tempStr.section("\'", 5, 5).toFloat();
                    QMap<QString, Device*> tempDevices;
                    QString deviceStr = tempStr.section("\'", 7, 7);
                    for (int i = 0; !tempStr.section(",", i,i).isEmpty(); i++) {
                        tempDevices.insert(deviceStr.section(",", i,i), &(devices->getDevices().find(deviceStr.section(",", i,i)).value()));
                    }
                    qDebug() << "Camera with name = " << tempName << ", x = " << x << ", y = " << y << ", devices: " << tempDevices << " found";

                    cameras->insert(tempName, Camera(tempName, tempDevices));
                }
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

//работало бы если бы мы получали инфо о координатах людей, но как то не задалось с этим........
/*void Map::checkPositions() {
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

}   */

void Map::setSender(Client *newClient) {
    client = newClient;
}

void Map::setDevices(Devices *newDevices)  {
    devices = newDevices;
}

void Map::slotProcessData(QString data) {
    qDebug() << "Processing data ...";

    qDebug() << "Done.";
}
