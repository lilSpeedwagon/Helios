#include "person.h"

Person::Person()
{

}

Person::Person(quint16 id, QString name, Point position, Access access) : id(id), name(name), position(position), access(access)   {}

Person::Person(Person const& p) {
    id = p.getId();
    name = p.getName();
    position = p.getPosition();
    access = p.getAccess();
}

void Person::operator=(Person const& p) {
    id = p.getId();
    name = p.getName();
    position = p.getPosition();
    access = p.getAccess();
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

Person::Access Person::getAccess() const    {
    return access;
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

void Person::setAccess(Access access)   {
    this->access = access;
}
