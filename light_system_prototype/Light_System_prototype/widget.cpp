#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "-----------------------------";
    qDebug() << "window initializating...";

    resize(MIN_WIDTH, MIN_HEIGHT);

    mainLayout = new QHBoxLayout();
    mainLayout->setMargin(ITEM_MARGIN);

    lightButtons = new QMap<QString, QPushButton*>();

    initLightTable();
    initOptions();

    setLayout(mainLayout);

    initTimer();

    setStatus(Status::READY);
    qDebug() << "Done. Window has been initialized";
}

void Widget::initLightTable()   {
    lightTableLayout = new QHBoxLayout();

    lightTable = new QTableWidget(1, 5, this);
    lightTable->setItem(0, 0, new QTableWidgetItem("#"));
    lightTable->setItem(0, 1, new QTableWidgetItem("Name"));
    lightTable->setItem(0, 2, new QTableWidgetItem("Adress"));
    lightTable->setItem(0, 3, new QTableWidgetItem("Position"));
    lightTable->setItem(0, 4, new QTableWidgetItem("State"));
    lightTable->setRowHeight(0, ROW_HEIGHT);
    lightTable->setMinimumSize(COLUMN_WIDTH * 5, ROW_HEIGHT * 2);
    lightTable->setDisabled(true);

    lightButtonLayout = new QVBoxLayout();
    lightButtonLayout->setContentsMargins(0,0,0,0);
    lightTableLayout->addWidget(lightTable);
    lightTableLayout->addLayout(lightButtonLayout);


    mainLayout->addLayout(lightTableLayout);
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

    networkPortLabel = new QLabel("Network port");
    optionsLayout->addWidget(networkPortLabel, 1, Qt::AlignTop);

    networkPortBox = new QLineEdit(QString::number(Client::PORT_DEFAULT));
    networkPortBox->setPlaceholderText("Set yout network port...");
    optionsLayout->addWidget(networkPortBox, 1, Qt::AlignTop);

    callLayout = new QHBoxLayout();

    callDevicesButton = new QPushButton("Refresh Devices");
    connect(callDevicesButton, SIGNAL(clicked()), this, SLOT(slotCallDevicesButton()));
    callLayout->addWidget(callDevicesButton, 1, Qt::AlignTop);

    cancelCallButton = new QPushButton("Cancel");
    connect(cancelCallButton, SIGNAL(clicked()), this, SLOT(slotCancelCallButton()));
    cancelCallButton->setEnabled(false);
    callLayout->addWidget(cancelCallButton, 1, Qt::AlignTop);

    optionsLayout->addLayout(callLayout);

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

void Widget::createButtons()    {
    qDebug() << "creating buttons...";
    for (QPushButton *button : *lightButtons)   {
        lightButtonLayout->removeWidget(button);
    }
    lightButtons->clear();

    int i = 0;
    for (Device& device : devices->getDevices())    {
        device.setId(i);
        QString name = device.getName();
        QPushButton *button = new QPushButton("ON");
        //button->setEnabled(false);
        lightButtons->insert(name, button);
        //button->setGeometry(ITEM_MARGIN * 2 + COLUMN_WIDTH * 5, ROW_HEIGHT * (i++) + ITEM_MARGIN, COLUMN_WIDTH, ROW_HEIGHT);
        button->move(ITEM_MARGIN * 2 + COLUMN_WIDTH * 5, ROW_HEIGHT * (i++) + ITEM_MARGIN);

        QString message = device.isEnabled() ? Client::MESSAGE_OFF : Client::MESSAGE_ON;

        connect(button, SIGNAL(clicked()), this, SLOT(slotSwitchButton()));

        lightButtonLayout->addWidget(button);
    }
    qDebug() << "Done.";
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
        case Status::MAP:   {
            statusBar->setText("Status: Map choosing...");
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
    //lightTable->resize(COLUMN_WIDTH * 5, ROW_HEIGHT * lightTable->rowCount());
}

void Widget::slotRefreshDevicesData()   {
    int i = 1;
    lightTable->setRowCount(devices->getDevices().size() + 1);

    for (Device &device : devices->getDevices())    {
        lightTable->setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        lightTable->setItem(i, 1, new QTableWidgetItem(device.getName()));
        lightTable->setItem(i, 2, new QTableWidgetItem(device.getAdress()));
        lightTable->setItem(i, 3, new QTableWidgetItem(QString::number(device.getPosition().X()) + " : " + QString::number(device.getPosition().Y())));
        lightTable->setItem(i, 4, new QTableWidgetItem(device.getPowerStateStr()));

        lightButtons->find(device.getName()).value()->setEnabled(device.isConnected());
        lightButtons->find(device.getName()).value()->setText(device.isEnabled() ? "OFF" : "ON");

        i++;
    }
    if (client != nullptr && status == Status::CALLING)  {
        /*  if (!client->isCalling() && !callDevicesButton->isEnabled())    {
            callDevicesButton->setEnabled(true);
            setStatus(Status::READY);
        }   */
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
    connect(this->client, SIGNAL(signalEndCalling()), this, SLOT(slotEndCalling()), Qt::ConnectionType::DirectConnection);
    connect(this, SIGNAL(signalCancelCalling()), this->client, SLOT(slotCancelCalling()), Qt::ConnectionType::DirectConnection);
}

void Widget::setDevices(Devices *devices)   {
    this->devices = devices;

    createButtons();
}

void Widget::slotCallDevicesButton()    {
    QString adress = networkAdressBox->text();
    if (!adress.endsWith('.'))
        adress.append('.');
    quint16 port = networkPortBox->text().toInt();

    if (Check::isCorrectAdress(adress) && port)    {
        if (client != nullptr)  {
            setStatus(Status::CALLING);
            callDevicesButton->setEnabled(false);
            cancelCallButton->setEnabled(true);

            client->setAdress(adress, port);
            devices->clearAdresses();
            //client->callDevicesInThread();
            client->callDevices();
        }   else    {
            qDebug() << "Trying to recall devices, but client isn't set for this widget.";
        }
    }   else    {
        qDebug() << "Incorrect adress or port";
        QMessageBox::warning(this, "Warning", "Incorrect adress or port");
    }
}


void Widget::slotCancelCallButton() {
    if (status == Status::CALLING) {
        qDebug() << "Calling was canceled by user.";
        emit signalCancelCalling();
        cancelCallButton->setEnabled(false);
        setStatus(Status::READY);
    }
}

void Widget::slotEndCalling()   {
    qDebug() << "end calling slot";
    setStatus(Status::READY);
    callDevicesButton->setEnabled(true);
    cancelCallButton->setEnabled(false);
}

void Widget::slotFileChooseButton() {
    qDebug() << "-----------------------------";
    qDebug() << "Choosing map file...";
    setStatus(Status::MAP);
    QFileDialog fileDialog;
    QString mapFile = fileDialog.getOpenFileName();
    if (!Map::getMap().readMap(mapFile)) {
        QMessageBox::critical(this, "Error", "Map file not fount or it's incorrect.");
    }
    createButtons();
    setStatus(Status::READY);
}

void Widget::slotSwitchButton()   {
    QPushButton *button = (QPushButton*) sender();

    QString name = lightButtons->key(button);

    qDebug() << "slot switch button " << name;

    Device &device = devices->getDevices().find(name).value();

    QString message;
    if (device.getPowerState() == PowerState::ENABLED) {
        message = Client::MESSAGE_OFF;
    }   else    {
        message = Client::MESSAGE_ON;
    }
    if (!client->sendToDevice(device, message)) {
        QMessageBox::critical(this, "Error", "Lost connection to device " + name);
    }
}
