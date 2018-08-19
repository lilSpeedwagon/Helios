#include "map.h"

//статический член класса, согласно стандарту, должен быть определен где то вне класса
Map* Map::map;

Map::Map()
{
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
    mapFile = new QFile("C:\\docs\\programms\\Future Gadgets LAb\\Smart_district\\Helios\\light_system_prototype\\smartDistrict.map");
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
                Devices::getDevices()->insert(tempName, tempDevice);
            }
            //здесь должно быть так же определение lock и геометрии карты
        }
        qDebug() << "Done. Map file has been successfuly red.";
    }   else    {
        qDebug() << "map file not found or it's incorrect :(";
        qDebug() << "Aborting programm.";
        exit(1);
    }
    qDebug() << "------------------------------------";
}
