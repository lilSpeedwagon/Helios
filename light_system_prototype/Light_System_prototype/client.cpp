#include "client.h"
//private
const QString Client::ADRESS_MASK_DEFAULT = "10.1.13";
//const QString Client::ADRESS_MASK_DEFAULT = "192.168.43";
const quint16 Client::PORT_DEFAULT = 8888;

const int Client::ADRESS_MIN = 80;
const int Client::ADRESS_MAX = 150;

//public
const quint16 Client::DELAY_CONNECTION = 70;
const quint16 Client::DELAY_READ_DATA = 1000;

const QString Client::MESSAGE_CALL = "HI";
const QString Client::MESSAGE_ON = "ON";
const QString Client::MESSAGE_OFF = "OFF";
const QString Client::MESSAGE_ASK = "HOW";

void Client::setSocket(QTcpSocket& socket)  {
    connect(&socket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(&socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(&socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
}

Client::Client(Devices* devices)
{
    qDebug() << "-----------------------------";
    qDebug() << "Client initializing...";

    this->devices = devices;
    socket = new QTcpSocket();
    setSocket(*socket);

    qDebug() << "Done.";
}

void Client::callDevicesInThread()  {
    QtConcurrent::run(this, &Client::callDevices);
}

void Client::callDevices() {
    qDebug() << "Calling for devices on local adress " << adress << " and port " << port << "...";

    if (socket != nullptr)  {
        delete socket;
        socket = new QTcpSocket();
        setSocket(*socket);
    }

    deviceNumber = 0;
    progress = 0;
    callingForDevices = true;
    for (int i = ADRESS_MIN; i <= ADRESS_MAX; i++)  {
        if (callingForDevices)   {
            progress = i;
            currentAdress = adress + QString::number(i);
            call(currentAdress, PORT_DEFAULT);
        }   else    {
            progress = 0;
        }

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

void Client::slotCancelCalling()    {
    callingForDevices = false;
}

void Client::slotConnected()    {
    qDebug() << "Connected.";
}

void Client::slotError(QAbstractSocket::SocketError error)    {
    qDebug() << error;
}

void Client::slotReadyRead()    {

    if (socket->bytesAvailable())   {
        QByteArray bytes = socket->readAll();

        QString message(bytes);
        if (message.endsWith("\r\n"))
            message.chop(2);
        qDebug() << "incoming data: " << message;

        if (callingForDevices)  {
            for (Device &device : devices->getDevices())   {
                if (device.getName() == message)  {
                    qDebug() << "Device " << message << "connected.";
                    device.setAdress(currentAdress);
                    deviceNumber++;
                }
            }
        }

        if (message == "DONE")  {
            for (Device &device : devices->getDevices())    {
                if (device.getAdress() == currentAdress)    {
                    device.changeState();
                }
            }
        }
    }   else    {
        qDebug() << "empty message.";
    }
}

void Client::send(QString message)  {
    qDebug() << "Sending " << message;
    if (socket == nullptr)  {
        socket = new QTcpSocket();
        setSocket(*socket);
    }
    if (socket->state() == QAbstractSocket::SocketState::ConnectedState)    {
        char *bytes = new char[message.length() + 2];
        bytes[0] = message.length();
        for (int i = 0; i < message.length(); i++)    {
            bytes[i + 1] = (char) message.data()[i].toLatin1();
        }
        bytes[message.length() + 1] = '\0';
        socket->write(bytes);

        qDebug() << "Successful sending.";
    }   else    {
        qDebug() << "Sending error. No current connections.";
    }
}

bool Client::sendToDevice(QString deviceName, QString message)  {
    if (devices->getDevices().contains(deviceName)) {
        return sendToDevice(devices->getDevices().take(deviceName), message);
    }   else    {
        qDebug() << "no " << deviceName << " device found";
        return false;
    }
}

bool Client::sendToDevice(Device device, QString message)    {
    return sendToDevice(device.getAdress(), PORT_DEFAULT, message);
}

bool Client::sendToDevice(QString adress, quint16 port, QString message)    {
    if (socket == nullptr)  {
        socket = new QTcpSocket();
        setSocket(*socket);
    }
    if (socket->state() == QAbstractSocket::SocketState::ConnectedState)    {
        socket->disconnectFromHost();
    }
    currentAdress = adress;
    socket->connectToHost(adress, port);
    if (socket->waitForConnected(DELAY_CONNECTION))
        send(message);
    else    {
        qDebug() << "Connection error";
        return false;
    }
    if (socket->waitForReadyRead(DELAY_READ_DATA))
        socket->disconnectFromHost();
    return true;
}

void Client::setAdress(QString const& adress)   {
    this->adress = adress;
}

void Client::setAdress(QString const& adress, quint16 const& port)  {
    this->adress = adress;
    this->port = port;
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

QTcpSocket* Client::getSocket() const    {
    if (socket != nullptr)
        return socket;
    return 0;
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

