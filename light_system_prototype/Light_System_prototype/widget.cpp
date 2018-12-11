#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    qDebug() << "-----------------------------";
    qDebug() << "window initializating...";

    resize(MIN_WIDTH, MIN_HEIGHT);
    setMinimumSize(QSize(MIN_WIDTH, MIN_HEIGHT));

    mainLayout = new QHBoxLayout();
    mainLayout->setMargin(ITEM_MARGIN);

    lightButtons = new QMap<QString, QPushButton*>();

    lightScrollArea = new QScrollArea(this);
    mainLayout->addWidget(lightScrollArea);
    initLightTable();
    initOptions();

    setLayout(mainLayout);

    initTimer();

    setStatus(Status::READY);
    qDebug() << "Done. Window has been initialized";
}

void Widget::initLightTable()   {
    lightLayout = new QGridLayout(lightScrollArea);
    lightLayout->setSpacing(5);

    lightLayout->addWidget(new QLabel("#"), 0, 0);
    lightLayout->addWidget(new QLabel("Name"), 0, 1);
    lightLayout->addWidget(new QLabel("Adress"), 0, 2);
    lightLayout->addWidget(new QLabel("Position"), 0, 3);
    lightLayout->addWidget(new QLabel("State"), 0, 4);
    lightLayout->addWidget(new QLabel("Button"), 0, 5);
    lightLayout->addWidget(new QLabel("Network"), 0, 6);

    for (int i = 0; i < 7; i++)
        lightLayout->setColumnMinimumWidth(i, COLUMN_WIDTH);
    lightLayout->setRowMinimumHeight(0, ROW_HEIGHT);
    lightLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
    lightLayout->setAlignment(Qt::AlignTop);

    lightScrollArea->setLayout(lightLayout);
}

void Widget::initOptions()  {
    optionsBox = new QGroupBox("Options");
    optionsBox->setMinimumWidth(MIN_OPTIONS_WIDTH);

    optionsLayout = new QVBoxLayout();
    optionsLayout->setAlignment(Qt::AlignTop);

    networkAdressLabel = new QLabel("Network adress");
    networkAdressLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    optionsLayout->addWidget(networkAdressLabel, 1, Qt::AlignTop);

    //networkAdressBox = new QLineEdit(QNetworkInterface::allAddresses().first().toString());
    networkAdressBox = new QLineEdit("");
    networkAdressBox->setPlaceholderText("Set yout network adress...");
    networkAdressBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    optionsLayout->addWidget(networkAdressBox, 1, Qt::AlignTop);

    callDevicesButton = new QPushButton("Refresh Devices");
    callDevicesButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(callDevicesButton, SIGNAL(clicked()), this, SLOT(slotCallDevicesButton()));
    optionsLayout->addWidget(callDevicesButton);

    fileChooseButton = new QPushButton("Map file...");
    fileChooseButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    connect(fileChooseButton, SIGNAL(clicked()), this, SLOT(slotFileChooseButton()));
    optionsLayout->addWidget(fileChooseButton, 1, Qt::AlignTop);

    statusBar = new QLabel("Inititalizing...");
    statusBar->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    optionsLayout->addWidget(statusBar, 1, Qt::AlignBottom);

    optionsBox->setLayout(optionsLayout);
    mainLayout->addWidget(optionsBox);
}

void Widget::createDeviceTable()    {
    qDebug() << "refreshing device table...";

    timer->stop();

    for (int i = 0; i < lightButtons->size() * 7; i++)
        delete lightLayout->takeAt(7)->widget();
    lightButtons->clear();

    int i = 1;
    for (Device& device : devices->getDevices())    {
        device.setId(i);
        QString name = device.getName();
        QPushButton *button = new QPushButton("ON", lightLayout->widget());
        lightButtons->insert(name, button);

        QString message = device.isEnabled() ? Client::MESSAGE_OFF : Client::MESSAGE_ON;

        connect(button, SIGNAL(clicked()), this, SLOT(slotSwitchButton()));

        lightLayout->addWidget(new QLabel(QString::number(i), lightLayout->widget()), i, 0);
        lightLayout->addWidget(new QLabel(device.getName(), lightLayout->widget()), i, 1);
        lightLayout->addWidget(new QLabel(device.getAdress(), lightLayout->widget()), i, 2);
        lightLayout->addWidget(new QLabel(QString::number(device.getPosition().X()) + " : " + QString::number(device.getPosition().Y()), lightLayout->widget()), i, 3);
        lightLayout->addWidget(new QLabel(device.getPowerStateStr(), lightLayout->widget()), i, 4);
        lightLayout->addWidget(button, i, 5);
        lightLayout->addWidget(new QLabel("Disconnected", lightLayout->widget()), i, 6);

        i++;
    }

    timer->start();

    qDebug() << "Done. Buttons has been created.";
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

void Widget::slotRefreshDevicesData()   {
    for (Device &device : devices->getDevices())    {
        dynamic_cast<QLabel*>(lightLayout->itemAtPosition(device.getId(), 2)->widget())->setText(device.getAdress());
        dynamic_cast<QLabel*>(lightLayout->itemAtPosition(device.getId(), 4)->widget())->setText(device.getPowerStateStr());
        dynamic_cast<QLabel*>(lightLayout->itemAtPosition(device.getId(), 6)->widget())->setText(device.getNetworkState());

        if (status != Status::CALLING)  {
            lightButtons->find(device.getName()).value()->setEnabled(device.isConnected());
            lightButtons->find(device.getName()).value()->setText(device.isEnabled() ? "OFF" : "ON");
        }

        if (device.isConnected())   {
            device.setAdress("");
            callManager->checkConnected(device);
        }
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
    if (timer != nullptr)   {
        timer->stop();
        delete timer;
    }
    qDebug() << "Done.";
}

void Widget::setCallManager(CallManager *callManager)  {
    this->callManager = callManager;
    connect(this->callManager, SIGNAL(signalEndCalling()), this, SLOT(slotEndCalling()), Qt::ConnectionType::QueuedConnection);
}

void Widget::setDevices(Devices *devices)   {
    this->devices = devices;
    createDeviceTable();
}

void Widget::slotCallDevicesButton()    {
    QString adress = networkAdressBox->text();
    if (adress.endsWith("1"))
        adress.chop(1);
    if (!adress.endsWith('.'))
        adress.append('.');

    if (Check::isCorrectAdress(adress))    {
        if (callManager != nullptr)  {
            setStatus(Status::CALLING);
            callDevicesButton->setEnabled(false);

            callManager->setAdress(adress);
            devices->clearAdresses();
            //client->callDevicesInThread();
            callManager->callDevices();
        }   else    {
            qDebug() << "Trying to recall devices, but client isn't set for this widget.";
        }
    }   else    {
        qDebug() << "Incorrect adress or port";
        QMessageBox::warning(this, "Warning", "Incorrect adress or port");
    }
}

void Widget::slotEndCalling()   {
    qDebug() << "end calling slot";
    setStatus(Status::READY);
    callDevicesButton->setEnabled(true);
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
    createDeviceTable();
    setStatus(Status::READY);
}

void Widget::slotSwitchButton()   {
    QPushButton *button = dynamic_cast<QPushButton*>(sender());

    QString name = lightButtons->key(button);

    qDebug() << "slot switch button " << name;

    Device &device = devices->getDevices().find(name).value();

    QString message;
    if (device.getPowerState() == PowerState::ENABLED) {
        message = CallManager::REQUEST_OFF;
    }   else    {
        message = CallManager::REQUEST_ON;
    }
    callManager->sendToDevice(device, message);
}
