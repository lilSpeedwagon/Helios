#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "device.h"
#include "devices.h"
#include "person.h"
#include "persons.h"
//#include "tcp.h"
#include "client.h"
#include "map.h"

#include <QNetworkConfigurationManager>

int main(int argc, char *argv[])
{
    qDebug() << "Initializing application...";
    QApplication a(argc, argv);

    Widget w;
    w.show();

    Devices::getDevices();
    Persons::getPersons();

    Map::getMap();

    //TCP::getTCP();
    Client::getClient();

    Client::getClient().sendToDevice(Devices::getDevices()->take("L12"), Client::MESSAGE_ON);


    qDebug() << "Application has been initialized.";

    return a.exec();
}


