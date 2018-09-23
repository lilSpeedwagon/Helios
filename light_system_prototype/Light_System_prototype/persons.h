#ifndef PERSONS_H
#define PERSONS_H

#include "person.h"
#include "point.h"
#include <QDebug>
#include <QMap>
#include <QString>

/* Singleton класс для хранения массива распознаных камерами гостей */

class Persons
{
private:
    static QMap<QString, Person> *persons_ptr;
    Persons();
    Persons(Persons const&);
    Persons& operator=(Persons const&);
public:
    static QMap<QString, Person> *getPersons();
    static void add(QString name, Person person);
    static void remove(QString name);
    static void setPosition(QString name, Point point);

    /* отдельные статические методы по сути просто являются оболочкой над обычными insert и remove для коллекций, но дополнительно вырабатывают сигналы для работы с GUI */
};

#endif // PERSONS_H
