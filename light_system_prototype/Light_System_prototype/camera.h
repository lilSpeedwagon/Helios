#ifndef CAMERA_H
#define CAMERA_H

#include <QString>
#include <QMap>
#include "point.h"
#include "device.h"

class Camera
{
private:
    int id;
    QString name;
    Point position; //а надо ли?
    QMap<QString, Device*> devices;

public:
    Camera();
    Camera(QString name, QMap<QString, Device*> devices);
    Camera(int id, QString name, QMap<QString, Device*> devices);
    Camera(Camera const& camera);
    void operator=(Camera const& camera);

    void setId(int id);
    void setName(QString name);
    void setPosition(Point positon);
    void setDevices(QMap<QString, Device*> &devices);

    int getId() const;
    QString getName() const;
    Point getPosition() const;
    QMap<QString, Device*>& getDevices();
};

#endif // CAMERA_H
