#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>
#include <QTableView>

class Widget : public QWidget
{
    Q_OBJECT

private:
    QHBoxLayout* mainLayout;
    QWidget* deviceList;
    QWidget* personList;
    QWidget* mapWidget;
    QTableView* lightTable;
public:
    Widget(QWidget *parent = 0);
    ~Widget();

public slots:

};

#endif // WIDGET_H
