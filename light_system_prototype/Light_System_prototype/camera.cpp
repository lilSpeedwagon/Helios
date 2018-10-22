#include "camera.h"

Camera::Camera() {}

Camera::Camera(QString name, QMap<QString, Device*> devices) : name(name), devices(devices) {}

Camera::Camera(int id, QString name, QMap<QString, Device*> devices) : id(id), name(name), devices(devices) {}

Camera::Camera(Camera const& camera) : id(camera.id), name(camera.name), devices(camera.devices)    {}

void Camera::operator=(Camera const& camera)    {
    id = camera.id;
    name = camera.name;
    devices = camera.devices;
}

void Camera::setId(int id)    {
    this->id = id;
}

void Camera::setName(QString name)  {
    this->name = name;
}

void Camera::setPosition(Point position)  {
    this->position = position;
}

void Camera::setDevices(QMap<QString, Device*> &devices)   {
    this->devices = devices;
}

int Camera::getId() const   {
    return id;
}

QString Camera::getName() const {
    return name;
}

Point Camera::getPosition() const   {
    return position;
}

QMap<QString, Device*>& Camera::getDevices()     {
    return devices;
}
