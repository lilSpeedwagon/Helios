#ifndef DEVICES_H
#define DEVICES_H

#include <QMap>
#include "device.h"

class Devices
{
private:
    QMap<QString, Device> *devices;
public:
    Devices();
    Devices(Devices const& );
    Devices& operator=(Devices const&);
    void add(QString name, Device const& device);
    void remove(QString name);
    void clear();
    void clearAdresses();
    QMap<QString, Device>& getDevices() const;
    int size() const;
signals:
    void signalNewDevice();
    void signalRemoveDevice();
};


#endif // DEVICES_H
