#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include "devices.h"
#include "device.h"

class Client : QWidget
{
    Q_OBJECT
private:
    static const QString ADRESS_MASK;
    static const quint16 PORT_DEFAULT;

    QTcpSocket *socket;
    QString currentAdress;
    bool callingForDevices = false;

    static Client *client;
    Client();
    Client(Client const& );
    void operator=(Client const& );
    void call(QString adress, quint16 port = 0);
    void callDevices();
    void send(QString message);

public:
    static const QString MESSAGE_ON;
    static const QString MESSAGE_OFF;
    static const QString MESSAGE_ASK;
    static const QString MESSAGE_CALL;

    static Client& getClient();
    void sendToDevice(Device device, QString message);
    void sendToDevice(QString adress, quint16 port, QString message);

public slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError);
    void slotReadyRead();
signals:
    void signalNewDevice();
};

#endif // CLIENT_H
