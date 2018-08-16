#include "client.h"

Client::Client() {
    printf("init client\n\r");
    socket = new QTcpSocket();
}

void Client::connectTo(QString hostName, quint16 port)  {
    printf("connecting to %s \n\r", new QString(hostName));
    socket->connectToHost(hostName, port);
    QObject::connect(socket, SIGNAL(connected()), this, SLOT(slotConnected()));
    QObject::connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError()));
}

void Client::send(QString message)  {
    printf("sending: %s", new QString(message));
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out << quint16(0) << message;

    out.device()->seek(0);
    out << quint16(arrBlock.size() - sizeof(quint16));

    socket->write(arrBlock);
}

void Client::slotConnected()    {
    printf("connected\n\r");
    send("oh, hi Mark");
}

void Client::slotError()    {
    printf("error");
}

void Client::slotReadData() {

}
