#include "persons.h"

Persons::Persons()
{

}

QMap<QString, Person>& Persons::getPersons()  {
    if (!persons_ptr)
        persons_ptr = new QMap<QString, Person>;
    return *persons_ptr;
}

void Persons::add(QString name, Person person)    {
    if (!persons_ptr)
        persons_ptr = new QMap<QString, Person>;
    persons_ptr->insert(name, person);
}

void Persons::remove(QString name) {
    persons_ptr->remove(name);
}

void Persons::setPosition(QString name, Point point)    {
    persons_ptr->take(name).setPosition(point);
}
