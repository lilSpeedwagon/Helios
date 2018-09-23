#include "client.h"

const QString Client::ADRESS_MASK = "192.0.0.";
const quint16 Client::PORT_DEFAULT = 80;
const QString Client::MESSAGE_CALL = "#HI#";
const QString Client::MESSAGE_ON = "#ON#";
const QString Client::MESSAGE_OFF = "#OFF#";
const QString Client::MESSAGE_ASK = "#HOW#";

//статический член класса, согласно стандарту, должен быть определен где то вне класса
Client* Client::client;

Client::Client()
{
    qDebug() << "-----------------------------";
    qDebug() << "Client initializing...";

    socket = new QTcpSocket();

    connect(socket, SIGNAL(connected()), this, SLOT(slotConnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    callDevices();

    qDebug() << "Client initialized";
}

void Client::callDevices() {
    qDebug() << "Calling for devices...";

    callingForDevices = true;
    for (size_t i = 0; i < 10; i++)  {
        currentAdress = ADRESS_MASK + QString::number(i);
        call(currentAdress, PORT_DEFAULT);
    }

    //currentAdress = "127.0.0.1";
    //call(currentAdress, 21);

    currentAdress.clear();
    if (socket->state() == QAbstractSocket::SocketState::ConnectedState)   {
        socket->disconnectFromHost();
    }

    callingForDevices = false;

    qDebug() << "Calling devices has finished.";
}

Client& Client::getClient() {
    if (!client)    {
        client = new Client();
    }
    return *client;
}

void Client::call(QString adress, quint16 port) {
    qDebug() << "Calling for " << adress << "...";
    socket->connectToHost(adress, port);
    if (socket->waitForConnected(1000)) {
        send(MESSAGE_CALL);
        socket->waitForReadyRead(5000);
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
                    socket->disconnectFromHost();
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
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        //out << quint16(0);
        out << message;
        out.device()->seek(0);
        //out << quint16(arrBlock.size() - sizeof(quint16));
        socket->write(arrBlock);
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
    socket->connectToHost(adress, port);
    if (socket->waitForConnected(1000))
        send(message);
    socket->disconnectFromHost();

}
