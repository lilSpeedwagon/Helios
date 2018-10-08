#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QDebug>
#include <QTcpSocket>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include "devices.h"
#include "device.h"

class Client : QObject
{
    Q_OBJECT
private:

    static const quint16 PORT_DEFAULT;
    static const quint16 DELAY_CONNECTION;
    static const quint16 DELAY_READ_DATA;

    Devices* devices;

    QTcpSocket *socket = nullptr;
    QString adress = ADRESS_MASK_DEFAULT;
    QString currentAdress;
    bool callingForDevices = false;

    void call(QString adress, quint16 port = 0);
    void send(QString message);

    int deviceNumber = 0;
    int progress = 0;
public:
    static const QString ADRESS_MASK_DEFAULT;
    static const QString MESSAGE_ON;
    static const QString MESSAGE_OFF;
    static const QString MESSAGE_ASK;
    static const QString MESSAGE_CALL;

    Client(Devices* devices);

    void callDevices();
    void callDevicesInThread();
    void sendToDevice(Device device, QString message);
    void sendToDevice(QString adress, quint16 port, QString message);
    void setAdress(QString const& adress);
    bool isConnected() const;
    bool isCalling() const;
    QString getCurrentAdress() const;
    QString getAdress() const;
    int getProgress() const;

    ~Client();

public slots:
    void slotConnected();
    void slotError(QAbstractSocket::SocketError);
    void slotReadyRead();
signals:
    void signalNewDevice();
    void signalEndCalling();
};

#endif // CLIENT_H
