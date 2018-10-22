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

    initLightTable();
    initOptions();

    setLayout(mainLayout);

    initTimer();

    setStatus(Status::READY);
    qDebug() << "Done. Window has been initialized";
}

void Widget::initLightTable()   {
    lightBox = new QGroupBox("Device list");

    lightLayout = new QGridLayout();
    lightLayout->setSpacing(5);

    lightLayout->addWidget(new QLabel("#"), 0, 0);
    lightLayout->addWidget(new QLabel("Name"), 0, 1);
    lightLayout->addWidget(new QLabel("Adress"), 0, 2);
    lightLayout->addWidget(new QLabel("Position"), 0, 3);
    lightLayout->addWidget(new QLabel("State"), 0, 4);
    lightLayout->addWidget(new QLabel("Button"), 0, 5);

    for (int i = 0; i < 6; i++)
        lightLayout->setColumnMinimumWidth(i, COLUMN_WIDTH);
    lightLayout->setRowMinimumHeight(0, ROW_HEIGHT);
    lightLayout->setSizeConstraint(QLayout::SizeConstraint::SetMinimumSize);
    lightLayout->setAlignment(Qt::AlignTop);

    lightBox->setLayout(lightLayout);
    mainLayout->addWidget(lightBox);
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

void Widget::createDeviceTable()    {
    qDebug() << "refreshing device table...";

    timer->stop();

    for (QPushButton *button : *lightButtons)   {
        lightLayout->removeWidget(button);
        delete button;
    }
    lightButtons->clear();

    int i = 1;
    for (Device& device : devices->getDevices())    {
        for (int j = 0; j <= 5; j++)  {
            lightLayout->removeWidget(dynamic_cast<QWidget*>(lightLayout->itemAtPosition(i, j)));
        }

        device.setId(i);
        QString name = device.getName();
        QPushButton *button = new QPushButton("ON");
        lightButtons->insert(name, button);

        QString message = device.isEnabled() ? Client::MESSAGE_OFF : Client::MESSAGE_ON;

        connect(button, SIGNAL(clicked()), this, SLOT(slotSwitchButton()));

        lightLayout->addWidget(new QLabel(QString::number(i)), i, 0);
        lightLayout->addWidget(new QLabel(device.getName()), i, 1);
        lightLayout->addWidget(new QLabel(device.getAdress()), i, 2);
        lightLayout->addWidget(new QLabel(QString::number(device.getPosition().X()) + " : " + QString::number(device.getPosition().Y())), i, 3);
        lightLayout->addWidget(new QLabel(device.getPowerStateStr()), i, 4);
        lightLayout->addWidget(button, i, 5);

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

        if (status != Status::CALLING)  {
            lightButtons->find(device.getName()).value()->setEnabled(device.isConnected());
            lightButtons->find(device.getName()).value()->setText(device.isEnabled() ? "OFF" : "ON");
        }
    }
    if (client != nullptr && status == Status::CALLING)  {
        recallProgressBar->setValue(client->getProgress());
    }   else    {
        recallProgressBar->setValue(0);
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

void Widget::setClient(Client *client)  {
    this->client = client;
    connect(this->client, SIGNAL(signalEndCalling()), this, SLOT(slotEndCalling()), Qt::ConnectionType::QueuedConnection);
    connect(this, SIGNAL(signalCancelCalling()), this->client, SLOT(slotCancelCalling()), Qt::ConnectionType::QueuedConnection);
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
    client->getSocket()->moveToThread(QThread::currentThread());
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
        message = Client::MESSAGE_OFF;
    }   else    {
        message = Client::MESSAGE_ON;
    }
    if (!client->sendToDevice(device, message))
        QMessageBox::critical(this, "Error", "Lost connection to device " + name);
}
