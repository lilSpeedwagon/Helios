#ifndef WIDGET_H
#define WIDGET_H

#include "device.h"
#include "devices.h"
#include "point.h"
#include "client.h"
#include "map.h"

#include <QWidget>
#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>
#include <QTableWidget>
#include <QTimer>
#include <QGroupBox>
#include <QLineEdit>
#include <QProgressBar>
#include <QFileDialog>

enum Status { INIT, CALLING, READY };

class Widget : public QWidget
{
    Q_OBJECT

private:
    const int MIN_WIDTH = 800;
    const int MIN_HEIGHT = 300;
    const int ROW_HEIGHT = 30;
    const int COLUMN_WIDTH = 100;
    const int ITEM_MARGIN = 10;
    const int MIN_OPTIONS_WIDTH = 200;
    const float REFRESH_FREQ = 1;

    void initLightTable();
    void initOptions();

    QHBoxLayout* mainLayout;
    QTableWidget* lightTable;
    QVBoxLayout* optionsLayout;
    QGroupBox* optionsBox;
    QLabel* networkAdressLabel;
    QLineEdit* networkAdressBox;
    QPushButton* callDevicesButton;
    QLabel* statusBar;
    QProgressBar* recallProgressBar;
    QPushButton* fileChooseButton;

    Status status = Status::INIT;
    void setStatus(Status const& status);

    QTimer* timer;
    void initTimer();
    void destroyTimer();

    Client* client;
    Devices* devices;

    bool isCorrectAdress(QString const& adress);
public:
    Widget(QWidget *parent = 0);
    void setClient(Client *client = nullptr);
    void setDevices(Devices *devices = nullptr);
    ~Widget();

public slots:
    void slotNewDevice(Device const& device);
    void slotRefreshDevicesData();
    void slotCallDevicesButton();
    void slotFileChooseButton();
};

#endif // WIDGET_H
