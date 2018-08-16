#ifndef PERSON_H
#define PERSON_H

#include <QString>
#include "point.h"

class Person
{
private:

    quint16 id;
    QString name;
    Point position;
public:
    Person();
    Person(quint16 id, QString name, Point position);
    Person(Person const& p);
    void operator=(Person const& p);
    quint16 getId() const;
    QString getName() const;
    Point getPosition() const;
    void setId(quint16 id);
    void setName(QString name);
    void setPosition(Point position);

};

#endif // PERSON_H
