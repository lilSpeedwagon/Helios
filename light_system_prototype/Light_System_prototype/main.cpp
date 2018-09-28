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
#include "coder.h"

#include <QNetworkConfigurationManager>

int main(int argc, char *argv[])
{
    qDebug() << "-----------------------------";
    qDebug() << "Initializing application...";
    QApplication a(argc, argv);

    Widget w;
    w.moveToThread(new QThread());
    w.show();


    Devices::getDevices();
    Persons::getPersons();

    Map::getMap();

    Client *client = new Client();
    client->callDevices();

    qDebug() << "Application has been initialized.";
    qDebug() << "-----------------------------";

    /* QString str = "Hi. My name is Connor. I'm an android sent by CyberLife.";
    qDebug() << str;
    str = Coder::encode(str);
    qDebug() << str;
    str = Coder::decode(str);
    qDebug() << str; */

    return a.exec();
}


