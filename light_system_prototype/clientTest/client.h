#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDataStream>

class Client : QObject
{
    Q_OBJECT

private:
    QTcpSocket *socket;
public:
    Client();
    void connectTo(QString hostName, quint16 port);
    void send(QString message);
public slots:
    void slotConnected();
    void slotError();
    void slotReadData();
};

#endif // CLIENT_H
