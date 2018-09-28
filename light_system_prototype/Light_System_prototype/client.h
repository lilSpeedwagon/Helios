#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include "devices.h"
#include "device.h"

class Client : QObject
{
    Q_OBJECT
private:
    static const QString ADRESS_MASK;
    static const quint16 PORT_DEFAULT;
    static const quint16 DELAY_CONNECTION;
    static const quint16 DELAY_READ_DATA;

    QTcpSocket *socket;
    QString currentAdress;
    bool callingForDevices = false;

    void call(QString adress, quint16 port = 0);
    void send(QString message);

    QThread *thread;

public:
    static const QString MESSAGE_ON;
    static const QString MESSAGE_OFF;
    static const QString MESSAGE_ASK;
    static const QString MESSAGE_CALL;

    Client();
    Client(Client const& );
    void operator=(Client const& );

    void callDevices();
    void sendToDevice(Device device, QString message);
    void sendToDevice(QString adress, quint16 port, QString message);
    bool isConnected() const;
    QString getCurrentAdress() const;

    ~Client();

public slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError);
    void slotReadyRead();
signals:
    void signalNewDevice();
};

#endif // CLIENT_H
