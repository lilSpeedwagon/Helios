#include "client.h"

const QString Client::ADRESS_MASK_DEFAULT = "192.168.1.";
const quint16 Client::PORT_DEFAULT = 8080;

const quint16 Client::DELAY_CONNECTION = 50;
const quint16 Client::DELAY_READ_DATA = 500;

const QString Client::MESSAGE_CALL = "HI";
const QString Client::MESSAGE_ON = "ON";
const QString Client::MESSAGE_OFF = "OFF";
const QString Client::MESSAGE_ASK = "HOW";

Client::Client(Devices* devices)
{
    qDebug() << "-----------------------------";
    qDebug() << "Client initializing...";

    this->devices = devices;
    socket = new QTcpSocket();

    connect(socket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    qDebug() << "Done.";
}

void Client::callDevicesInThread()  {
    QtConcurrent::run(this, &Client::callDevices);
}

void Client::callDevices() {
    qDebug() << "Calling for devices...";

    if (socket != nullptr)  {
        delete socket;
        socket = new QTcpSocket();
    }

    deviceNumber = 0;
    progress = 0;
    callingForDevices = true;
    for (int i = 0; i <= 255; i++)  {
        progress = i;
        currentAdress = adress + QString::number(i);
        call(currentAdress, PORT_DEFAULT);
    }

    currentAdress.clear();
    if (socket->state() == QAbstractSocket::SocketState::ConnectedState)   {
        socket->disconnectFromHost();
    }

    callingForDevices = false;

    emit signalEndCalling();
    qDebug() << "Calling devices has finished.";
    qDebug() << deviceNumber << " devices founded";
}

void Client::call(QString adress, quint16 port) {
    qDebug() << "Calling for " << adress << "...";
    socket->connectToHost(adress, port);
    if (socket->waitForConnected(DELAY_CONNECTION)) {
        send(MESSAGE_CALL);
        socket->waitForReadyRead(DELAY_READ_DATA);
        socket->disconnectFromHost();
    }    else    {
        qDebug() << "Connection error.";
    }
}

void Client::slotConnected()    {
    qDebug() << "Connected.";
}

void Client::slotError(QAbstractSocket::SocketError error)    {
    qDebug() << error;
}

void Client::slotReadyRead()    {
    qDebug() << "incoming data:";

    if (socket->bytesAvailable())   {
        QByteArray bytes = socket->readAll();

        QString message(bytes);
        if (message.endsWith("\r\n"))
            message.chop(2);
        qDebug() << message;

        if (callingForDevices)  {
            for (Device &device : devices->getDevices())   {
                if (device.getName() == message)  {
                    qDebug() << "Device " << message << "connected.";
                    device.setAdress(currentAdress);
                    deviceNumber++;
                }
            }
        }
    }   else    {
        qDebug() << "empty message.";
    }
}

void Client::send(QString message)  {
    qDebug() << "Sending " << message;
    if (socket == nullptr)
        socket = new QTcpSocket();
    if (socket->state() == QAbstractSocket::SocketState::ConnectedState)    {
        char *bytes = new char[message.length() + 2];
        bytes[0] = message.length();
        for (int i = 0; i < message.length(); i++)    {
            bytes[i + 1] = (char) message.data()[i].toLatin1();
        }
        bytes[message.length() + 1] = '\0';
        socket->write(bytes);

        qDebug() << "Done.";
    }   else    {
        qDebug() << "Sending error. No current connections.";
    }
}

void Client::sendToDevice(Device device, QString message)    {
    sendToDevice(device.getAdress(), PORT_DEFAULT, message);
}

void Client::sendToDevice(QString adress, quint16 port, QString message)    {
    if (socket == nullptr)
        socket = new QTcpSocket();
    if (socket->state() == QAbstractSocket::SocketState::ConnectedState)    {
        socket->disconnectFromHost();
    }
    currentAdress = adress;
    socket->connectToHost(adress, port);
    if (socket->waitForConnected(DELAY_CONNECTION))
        send(message);
    socket->disconnectFromHost();

}

void Client::setAdress(QString const& adress)   {
    this->adress = adress;
}

bool Client::isConnected() const {
    return (socket != nullptr && socket->state() == QAbstractSocket::SocketState::ConnectedState);
}

bool Client::isCalling() const   {
    return callingForDevices;
}

QString Client::getCurrentAdress() const {
    return currentAdress;
}

QString Client::getAdress() const   {
    return adress;
}

int Client::getProgress() const {
    return progress;
}

Client::~Client()   {
    qDebug() << "Destroying client...";
    if (socket != nullptr)  {
        if (socket->state() == QAbstractSocket::SocketState::ConnectedState)    {
            socket->disconnectFromHost();
        }
        delete socket;
    }
}
