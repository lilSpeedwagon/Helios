#include "persons.h"

//статический член класса, согласно стандарту, должен быть определен где то вне класса
QMap<QString, Person>* Persons::persons_ptr;

QMap<QString, Person>* Persons::getPersons()  {
    if (!persons_ptr)   {
        qDebug() << "-----------------------------";
        qDebug() << "persons initializing...";
        persons_ptr = new QMap<QString, Person>;
        qDebug() << "Done";
    }
    return persons_ptr;
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
