#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "device.h"
#include "devices.h"
#include "person.h"
#include "persons.h"
#include "client.h"
#include "callmanager.h"
#include "map.h"

#include <QHostAddress>
#include <QNetworkInterface>
#include <QNetworkConfigurationManager>

int main(int argc, char *argv[])
{
    qDebug() << "-----------------------------";
    qDebug() << "Initializing application...";
    QApplication a(argc, argv);

    Widget w;
    w.show();

    Devices *devices = new Devices();

    CallManager *callManager = new CallManager(devices);

    Map::getMap().setDevices(devices);
    Map::getMap().readMap("C:\\docs\\programms\\Future Gadgets LAb\\Smart_district\\Helios\\light_system_prototype\\smartDistrict.lmap");
    //Map::setSender(callManager);

    w.setCallManager(callManager);
    w.setDevices(devices);

    RequestManager manager;

    qDebug() << "Application has been initialized.";
    qDebug() << "-----------------------------";


    /* qDebug() << "adress list:";
    QString currenthost;
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address !=QHostAddress(QHostAddress::LocalHost)) {
            currenthost = address.toString();
            qDebug() << currenthost << " is it yours wifi network???";
        }
    } */

    return a.exec();
}


