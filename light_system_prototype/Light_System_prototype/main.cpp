#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "device.h"
#include "devices.h"
#include "person.h"
#include "persons.h"
#include "client.h"
#include "map.h"
#include "requestmanager.h"

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
    Persons::getPersons();

    Client *client = new Client(devices);

    Map::setDevices(devices);
    Map::setSender(client);
    Map::getMap();

    w.setClient(client);
    w.setDevices(devices);
    //client->callDevicesInThread();

    qDebug() << "Application has been initialized.";
    qDebug() << "-----------------------------";

    /*
    qDebug() << "adress list:";
    QList<QHostAddress> addressList = QNetworkInterface::allAddresses();
        foreach( QHostAddress address, addressList )
            qDebug( "yet another address: %s", qPrintable( address.toString() ) ); // выводим на экран
    */

    //RequestManager manager;
    //manager.sendGETRequest("http://localhost/get_test/get_test.php", "?request=answer");

    return a.exec();
}


