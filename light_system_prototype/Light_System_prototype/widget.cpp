#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "-----------------------------";
    qDebug() << "window initializating...";
    mainLayout = new QHBoxLayout();
    resize(1000, 400);
    lightTable = new QTableWidget(1, 5, this);
    lightTable->setItem(0, 0, new QTableWidgetItem("#"));
    lightTable->setItem(0, 1, new QTableWidgetItem("Name"));
    lightTable->setItem(0, 2, new QTableWidgetItem("Adress"));
    lightTable->setItem(0, 3, new QTableWidgetItem("Position"));
    lightTable->setItem(0, 4, new QTableWidgetItem("State"));
    lightTable->setRowHeight(0, ROW_HEIGHT);
    lightTable->setMinimumSize(COLUMN_WIDTH * 5 + 50, ROW_HEIGHT * 2 + 50);
    lightTable->setDisabled(true);
    //personList = new QWidget();
    //mapWidget = new QWidget();
    setLayout(mainLayout);

    //slotNewDevice(Device("device1", "192.0.0.1", Point(10, 100)));
    //slotNewDevice(Device("device2", "192.0.10.1", Point(50, 100)));

    //connect(Devices::getDevices(), SIGNAL(Devices::signalNewDevice()), this, SLOT(slotNewDevice()));

    initTimer();

    qDebug() << "Done. Window has been initialized";
}

Widget::~Widget()
{

}

void Widget::slotNewDevice(Device const& device)    {
    lightTable->setRowCount(lightTable->rowCount() + 1);
    lightTable->setRowHeight(lightTable->rowCount() - 1, ROW_HEIGHT);
    lightTable->setItem(lightTable->rowCount() - 1, 0, new QTableWidgetItem(QString::number(lightTable->rowCount() - 1)));
    lightTable->setItem(lightTable->rowCount() - 1, 1, new QTableWidgetItem(device.getName()));
    lightTable->setItem(lightTable->rowCount() - 1, 2, new QTableWidgetItem(device.getAdress()));
    lightTable->setItem(lightTable->rowCount() - 1, 3, new QTableWidgetItem(QString::number(device.getPosition().X()) + " : " + QString::number(device.getPosition().Y())));
    lightTable->setItem(lightTable->rowCount() - 1, 4, new QTableWidgetItem(device.getPowerStateStr()));
    lightTable->resize(COLUMN_WIDTH * 5 + 50, ROW_HEIGHT * lightTable->rowCount() + 50);
}

void Widget::slotRefreshDevicesData()   {
    //qDebug() << "refreshing table data";
    int i = 1;
    lightTable->setRowCount(Devices::getDevices()->size() + 1);
    lightTable->resize(COLUMN_WIDTH * 5 + 50, ROW_HEIGHT * lightTable->rowCount() + 50);
    for (Device device : *Devices::getDevices())    {
        lightTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        lightTable->setItem(i, 1, new QTableWidgetItem(device.getName()));
        lightTable->setItem(i, 2, new QTableWidgetItem(device.getAdress()));
        lightTable->setItem(i, 3, new QTableWidgetItem(QString::number(device.getPosition().X()) + " : " + QString::number(device.getPosition().Y())));
        lightTable->setItem(i, 4, new QTableWidgetItem(device.getPowerStateStr()));
        i++;
    }
}

void Widget::initTimer()   {
    qDebug() << "initializing GUI timer...";
    timer = new QTimer();
    timer->setInterval((int) 1000 / REFRESH_FREQ);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotRefreshDevicesData()));
    timer->start();
    qDebug() << "Done";
}

void Widget::destroyTimer() {
    qDebug() << "destroying GUI timer...";
    timer->stop();
    delete timer;
    qDebug() << "Done.";
}
