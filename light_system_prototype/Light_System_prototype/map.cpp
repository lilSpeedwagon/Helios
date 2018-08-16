#include "map.h"

Map::Map()
{
    qDebug() << "Map init...";
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
    // чтение карты из файла
}
