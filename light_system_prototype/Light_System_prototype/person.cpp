#include "person.h"

Person::Person()
{

}

Person::Person(quint16 id, QString name, Point position) : id(id), name(name), position(position)   {}

Person::Person(Person const& p) {
    id = p.getId();
    name = p.getName();
    position = p.getPosition();
}

void Person::operator=(Person const& p) {
    id = p.getId();
    name = p.getName();
    position = p.getPosition();
}

quint16 Person::getId() const {
    return id;
}

QString Person::getName() const   {
    return name;
}

Point Person::getPosition() const {
    return position;
}

void Person::setId(quint16 id)  {
    this->id = id;
}

void Person::setName(QString name)  {
    this->name = name;
}

void Person::setPosition(Point position)    {
    this->position = position;
}

