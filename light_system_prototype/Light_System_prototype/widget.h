#ifndef WIDGET_H
#define WIDGET_H

#include "device.h"
#include "devices.h"
#include "point.h"
#include "client.h"
#include "map.h"
#include "check.h"

#include <QWidget>
#include <QDebug>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QStatusBar>
#include <QTableWidget>
#include <QTimer>
#include <QGroupBox>
#include <QLineEdit>
#include <QProgressBar>
#include <QFileDialog>
#include <QMessageBox>

enum Status { INIT, CALLING, READY, MAP };

class Widget : public QWidget
{
    Q_OBJECT

private:
    const int MIN_WIDTH = 900;
    const int MIN_HEIGHT = 300;
    const int ROW_HEIGHT = 30;
    const int COLUMN_WIDTH = 100;
    const int ITEM_MARGIN = 10;
    const int MIN_OPTIONS_WIDTH = 200;
    const float REFRESH_FREQ = 5;

    void initLightTable();
    void initOptions();
    void createDeviceTable();
    void clearLightLayout();

    QHBoxLayout* mainLayout;

    QGroupBox* lightBox;
    QGridLayout* lightLayout;
    QMap<QString, QPushButton*> *lightButtons;

    QVBoxLayout* optionsLayout;
    QGroupBox* optionsBox;

    QLabel* currentNetworkAdressLabel;
    QLabel* networkAdressLabel;
    QLineEdit* networkAdressBox;

    QLabel* networkPortLabel;
    QLineEdit* networkPortBox;

    QHBoxLayout* callLayout;
    QPushButton* callDevicesButton;
    QPushButton* cancelCallButton;
    QProgressBar* recallProgressBar;
    QPushButton* fileChooseButton;

    QLabel* statusBar;

    Status status = Status::INIT;
    void setStatus(Status const& status);

    QTimer* timer;
    void initTimer();
    void destroyTimer();

    Client* client;
    Devices* devices;

public:
    Widget(QWidget *parent = 0);
    void setClient(Client *client = nullptr);
    void setDevices(Devices *devices = nullptr);
    ~Widget() = default;

public slots:
    //void slotNewDevice(Device const& device);
    void slotRefreshDevicesData();
    void slotCallDevicesButton();
    void slotCancelCallButton();
    void slotEndCalling();
    void slotFileChooseButton();
    void slotSwitchButton();

signals:
    void signalCancelCalling();
};

#endif // WIDGET_H
