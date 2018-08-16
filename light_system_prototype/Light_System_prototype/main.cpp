#include "widget.h"
#include <QApplication>
#include <QDebug>
#include "device.h"
#include "tcp.h"

int main(int argc, char *argv[])
{
    qDebug() << "Initializing application...";
    QApplication a(argc, argv);
    Widget w;
    TCP &server = TCP::getTCP();
    //server.askAll(true);
    w.show();

    return a.exec();
}
