#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include "point.h"

class Person
{
public:
    /* Пока что условные уровни доступа. RED - все зоны. YELLOW - все, кроме RED. GREEN - только GREEN. */
    enum Access { GREEN, YELLOW, RED };
private:
    QString name;
    Point position;
    Access access;
public:
    Person();
    Person(QString name, Point position, Access access = GREEN);
    Person(Person const& p);
    void operator=(Person const& p);
    QString getName() const;
    Point getPosition() const;
    Access getAccess() const;
    void setName(QString name);
    void setPosition(Point position);
    void setAccess(Access access);
};

#endif // PERSON_H
