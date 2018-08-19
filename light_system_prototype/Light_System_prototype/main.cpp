#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "device.h"
#include "devices.h"
#include "tcp.h"
#include "map.h"

int main(int argc, char *argv[])
{
    qDebug() << "Initializing application...";
    QApplication a(argc, argv);
    Widget w;
    TCP *server = TCP::getTCP();
    //server.askAll(true);
    Map::getMap();
    qDebug() << "Application has been initialized.";

    w.show();

    return a.exec();
}
