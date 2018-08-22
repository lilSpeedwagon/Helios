#include "person.h"

Person::Person()
{

}

Person::Person(QString name, Point position, Access access) : name(name), position(position), access(access)   {}

Person::Person(Person const& p) {
    name = p.getName();
    position = p.getPosition();
    access = p.getAccess();
}

void Person::operator=(Person const& p) {
    name = p.getName();
    position = p.getPosition();
    access = p.getAccess();
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

void Person::setName(QString name)  {
    this->name = name;
}

void Person::setPosition(Point position)    {
    this->position = position;
}

void Person::setAccess(Access access)   {
    this->access = access;
}
