#include "client.h"

const QString Client::ADRESS_MASK = "192.168.1.";
const quint16 Client::PORT_DEFAULT = 8888;

const quint16 Client::DELAY_CONNECTION = 50;
const quint16 Client::DELAY_READ_DATA = 500;

const QString Client::MESSAGE_CALL = "HI";
const QString Client::MESSAGE_ON = "ON";
const QString Client::MESSAGE_OFF = "OFF";
const QString Client::MESSAGE_ASK = "HOW";

Client::Client()
{
    qDebug() << "-----------------------------";
    qDebug() << "Client initializing...";

    socket = new QTcpSocket();

    connect(socket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    qDebug() << "Client initialized";
}

void Client::callDevices() {
    qDebug() << "Calling for devices...";

    callingForDevices = true;
    for (size_t i = 0; i < 255; i++)  {
        currentAdress = ADRESS_MASK + QString::number(i);
        call(currentAdress, PORT_DEFAULT);
    }

    /*currentAdress = "192.168.1.54";
    socket->connectToHost(currentAdress, 8888);
    socket->waitForConnected(DELAY_CONNECTION);
    send("HI");
    socket->waitForReadyRead(DELAY_READ_DATA);
    socket->disconnectFromHost();*/

    /*socket->connectToHost(currentAdress, 8888);
    for (int i = 0; i < 100; i++)   {
        send("ON");
        socket->waitForDisconnected(1000);
        send("OFF");
        socket->waitForDisconnected(1000);
    }*/

    /*send("$");
    socket->waitForDisconnected(3000);
    send("%"); */
    //socket->waitForReadyRead(25000);
    //call(currentAdress, 8888);

    currentAdress.clear();
    if (socket->state() == QAbstractSocket::SocketState::ConnectedState)   {
        socket->disconnectFromHost();
    }

    callingForDevices = false;

    qDebug() << "Calling devices has finished.";
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
            for (Device &device : *Devices::getDevices())   {
                if (device.getName() == message)  {
                    qDebug() << "Device " << message << "connected.";
                    device.setAdress(currentAdress);
                }
            }
        }
    }   else    {
        qDebug() << "empty message.";
    }
}

void Client::send(QString message)  {
    qDebug() << "Sending " << message;
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
    if (socket->state() == QAbstractSocket::SocketState::ConnectedState)    {
        socket->disconnectFromHost();
    }
    currentAdress = adress;
    socket->connectToHost(adress, port);
    if (socket->waitForConnected(DELAY_CONNECTION))
        send(message);
    socket->disconnectFromHost();

}

bool Client::isConnected() const {
    return (socket->state() == QAbstractSocket::SocketState::ConnectedState);
}

QString Client::getCurrentAdress() const {
    return currentAdress;
}

Client::~Client()   {
    qDebug() << "Destroying client...";
    if (socket->state() == QAbstractSocket::SocketState::ConnectedState)    {
        socket->disconnectFromHost();
    }
    delete socket;
}
