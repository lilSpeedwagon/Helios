#ifndef TCP_H
#define TCP_H

#include "device.h"
#include "devices.h"
#include <vector>
#include <QDebug>
#include <QTcpServer>
#include <QTcpSocket>
#include <QMap>
#include <QDataStream>
#include <QTime>
#include <QByteArray>
#include <QTimer>

#define PORT 80
#define MES_LENGTH 6    // message: # name(3) state(1) #

/* Класс для работы с TCP сервером. Содержит внутри всю логику подключения и отключения устройств,
обработки получаемых данных.  */

//Singleton class
class TCP : QObject
{
    Q_OBJECT
private:
    static TCP *tcp;
    TCP();
    TCP(TCP const&);
    TCP operator=(TCP const&);
    quint16 messageLength = MES_LENGTH;
    quint16 port = PORT;
    float frequency = 1;

    QTcpServer* server;
    QTimer *timer;
    //QMap<QString, Device> *devices;

    void initTimer();
    void send(QTcpSocket *socket, QString message) const;

public:
    static TCP &getTCP();
    QTcpServer *getServer() const;
    //QMap<QString, Device> *getDevices() const;
    //void setDevices(QMap<QString, Device> *devices);
    void sendToClient(Device device, QString message) const;
    void sendToAll(QString message) const;
    void askAll(bool value);
    void setFrequency(float value);
    float getFrequency() const;

public slots:
    void slotNewConnection();
    void slotReadClient();
    void slotDisconnected();
    void slotAskAll();
};

#endif // TCP_H
