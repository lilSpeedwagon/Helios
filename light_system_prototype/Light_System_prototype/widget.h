#ifndef WIDGET_H
#define WIDGET_H

#include "device.h"
#include "devices.h"
#include "person.h"
#include "persons.h"
#include "point.h"

#include <QWidget>
#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>
#include <QTableWidget>

class Widget : public QWidget
{
    Q_OBJECT

private:
    const int ROW_HEIGHT = 30;
    const int COLUMN_WIDTH = 100;

    QHBoxLayout* mainLayout;
    QWidget* deviceList;
    QWidget* personList;
    QWidget* mapWidget;
    QTableWidget* lightTable;
public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void slotNewDevice(Device const& device);
};

#endif // WIDGET_H
