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
    quint16 id;
    QString name;
    Point position;
    Access access;
public:
    Person();
    Person(quint16 id, QString name, Point position, Access access = GREEN);
    Person(Person const& p);
    void operator=(Person const& p);
    quint16 getId() const;
    QString getName() const;
    Point getPosition() const;
    Access getAccess() const;
    void setId(quint16 id);
    void setName(QString name);
    void setPosition(Point position);
    void setAccess(Access access);
};

#endif // PERSON_H
