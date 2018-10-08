#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "-----------------------------";
    qDebug() << "window initializating...";

    resize(MIN_WIDTH, MIN_HEIGHT);

    mainLayout = new QHBoxLayout();
    mainLayout->setMargin(ITEM_MARGIN);

    initLightTable();
    initOptions();

    setLayout(mainLayout);

    initTimer();

    setStatus(Status::READY);
    qDebug() << "Done. Window has been initialized";
}

void Widget::initLightTable(){
    lightTable = new QTableWidget(1, 5, this);
    lightTable->setItem(0, 0, new QTableWidgetItem("#"));
    lightTable->setItem(0, 1, new QTableWidgetItem("Name"));
    lightTable->setItem(0, 2, new QTableWidgetItem("Adress"));
    lightTable->setItem(0, 3, new QTableWidgetItem("Position"));
    lightTable->setItem(0, 4, new QTableWidgetItem("State"));
    lightTable->setRowHeight(0, ROW_HEIGHT);
    lightTable->setMinimumSize(COLUMN_WIDTH * 5, ROW_HEIGHT * 2);
    lightTable->setDisabled(true);
    mainLayout->addWidget(lightTable);
}

void Widget::initOptions()  {
    optionsBox = new QGroupBox("Options");
    optionsBox->setMinimumWidth(MIN_OPTIONS_WIDTH);

    optionsLayout = new QVBoxLayout();

    networkAdressLabel = new QLabel("Network adress");
    optionsLayout->addWidget(networkAdressLabel, 1, Qt::AlignTop);

    networkAdressBox = new QLineEdit(Client::ADRESS_MASK_DEFAULT);
    networkAdressBox->setPlaceholderText("Set yout network adress...");
    optionsLayout->addWidget(networkAdressBox, 1, Qt::AlignTop);

    callDevicesButton = new QPushButton("Refresh Devices");
    connect(callDevicesButton, SIGNAL(clicked()), this, SLOT(slotCallDevicesButton()));
    optionsLayout->addWidget(callDevicesButton, 1, Qt::AlignTop);

    recallProgressBar = new QProgressBar();
    recallProgressBar->setRange(0, 255);
    recallProgressBar->setValue(0);
    recallProgressBar->setEnabled(false);
    optionsLayout->addWidget(recallProgressBar, 1, Qt::AlignTop);

    fileChooseButton = new QPushButton("Map file...");
    connect(fileChooseButton, SIGNAL(clicked()), this, SLOT(slotFileChooseButton()));
    optionsLayout->addWidget(fileChooseButton, 1, Qt::AlignTop);

    statusBar = new QLabel("Inititalizing...");
    optionsLayout->addWidget(statusBar, 1, Qt::AlignBottom);

    optionsBox->setLayout(optionsLayout);
    mainLayout->addWidget(optionsBox);
}

Widget::~Widget()
{

}

void Widget::setStatus(Status const& status)    {
    this->status = status;
    switch (status) {
        case Status::CALLING:   {
            statusBar->setText("Status: Refreshing device list...");
            break;
        }
        case Status::READY: {
            statusBar->setText("Status: Ready");
            break;
        }
        case Status::INIT:  {
            statusBar->setText("Status: Initializing...");
            break;
        }
    }
}

void Widget::slotNewDevice(Device const& device)    {
    lightTable->setRowCount(lightTable->rowCount() + 1);
    lightTable->setRowHeight(lightTable->rowCount() - 1, ROW_HEIGHT);
    lightTable->setItem(lightTable->rowCount() - 1, 0, new QTableWidgetItem(QString::number(lightTable->rowCount() - 1)));
    lightTable->setItem(lightTable->rowCount() - 1, 1, new QTableWidgetItem(device.getName()));
    lightTable->setItem(lightTable->rowCount() - 1, 2, new QTableWidgetItem(device.getAdress()));
    lightTable->setItem(lightTable->rowCount() - 1, 3, new QTableWidgetItem(QString::number(device.getPosition().X()) + " : " + QString::number(device.getPosition().Y())));
    lightTable->setItem(lightTable->rowCount() - 1, 4, new QTableWidgetItem(device.getPowerStateStr()));
    lightTable->resize(COLUMN_WIDTH * 5, ROW_HEIGHT * lightTable->rowCount());
}

void Widget::slotRefreshDevicesData()   {
    //qDebug() << "refreshing table data";
    int i = 1;
    lightTable->setRowCount(devices->getDevices().size() + 1);
    //lightTable->resize(COLUMN_WIDTH * 5 + 50, ROW_HEIGHT * lightTable->rowCount() + 50);
    for (Device device : devices->getDevices())    {
        lightTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        lightTable->setItem(i, 1, new QTableWidgetItem(device.getName()));
        lightTable->setItem(i, 2, new QTableWidgetItem(device.getAdress()));
        lightTable->setItem(i, 3, new QTableWidgetItem(QString::number(device.getPosition().X()) + " : " + QString::number(device.getPosition().Y())));
        lightTable->setItem(i, 4, new QTableWidgetItem(device.getPowerStateStr()));
        i++;
    }
    if (client != nullptr)  {
        if (!client->isCalling() && !callDevicesButton->isEnabled())    {
            callDevicesButton->setEnabled(true);
            setStatus(Status::READY);
        }
        recallProgressBar->setValue(client->getProgress());
    }
}

void Widget::initTimer()   {
    qDebug() << "initializing GUI timer...";
    timer = new QTimer();
    timer->setInterval(1000 / REFRESH_FREQ);
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

void Widget::setClient(Client *client)  {
    this->client = client;
}

void Widget::setDevices(Devices *devices)   {
    this->devices = devices;
}

bool Widget::isCorrectAdress(QString const& adress) {
    int dotNumber = 0;
    for (int i = 0; i < adress.length(); i++)   {
        if (adress[i] == '.')
            dotNumber++;
    }
    if (dotNumber == 3) {
        int numberCount = 0;
        for (int i = 0; i < 3; i++) {
            bool ok;
            int num = adress.section('.', i, i).toInt(&ok);
            if (num >= 0 && num < 256 && ok)
                numberCount++;
        }
        if (numberCount == 3)
            return true;
    }
    return false;
}

void Widget::slotCallDevicesButton()    {
    QString adress = networkAdressBox->text();
    if (!adress.endsWith('.'))
        adress.append('.');

    if (isCorrectAdress(adress))    {
        if (client != nullptr)  {
            setStatus(Status::CALLING);
            callDevicesButton->setEnabled(false);
            client->setAdress(adress);
            client->callDevicesInThread();
        }   else    {
            qDebug() << "Trying to recall devices, but client isn't set for this widget.";
        }
    }   else    {
        qDebug() << "Incorrect adress";
    }
}

void Widget::slotFileChooseButton() {
    qDebug() << "-----------------------------";
    qDebug() << "Choosing map file...";
    QFileDialog fileDialog;
    QString mapFile = fileDialog.getOpenFileName();
    Map::getMap().readMap(mapFile);
}
