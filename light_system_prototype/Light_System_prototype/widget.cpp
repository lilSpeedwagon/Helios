#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "window initializating...";
    mainLayout = new QHBoxLayout();
    lightTable = new QTableView();
    lightTable->
    lightList = new
    personList = new QWidget();
    mapWidget = new QWidget();
    setLayout(mainLayout);
    qDebug() << "Done. Window has been initialized";
}

Widget::~Widget()
{

}
