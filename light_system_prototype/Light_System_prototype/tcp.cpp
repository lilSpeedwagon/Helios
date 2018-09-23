#include "tcp.h"

//статический член класса, согласно стандарту, должен быть определен где то вне класса
TCP* TCP::tcp_p;

/* Конструктор класса сервера */
TCP::TCP()  {
    qDebug() << "-----------------------------";
    qDebug() << "initializing TCP...";
    QTcpServer *serv = new QTcpServer();
    if (!serv->listen(QHostAddress::Any, port)) {
        qCritical() << "Server Error. Unable to start the server" + serv->errorString();
        serv->close();
    }   else    {
        qDebug() << "Server is ready";
        TCP::connect(serv, SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
        //devices = new QMap<QString, Device>();
    }
}

/* Экземпляр TCP сервера получаем через геттер, т.к. данный класс реализует шаблон Singleton */
TCP* TCP::getTCP()    {
    if (!tcp_p)
        tcp_p = new TCP();
    return tcp_p;
}

/* Можем получить внутренний класс сервера на всякий случай */
QTcpServer* TCP::getServer() const   {
    return server;
}

/* Метод для оптравки message на устройство device */
/*void TCP::sendToClient(Device device, QString message) const {
    qDebug() << "sending " + message + " to " + device.getName() + "...";
    QTcpSocket *socket = device.getSocket();
    send(socket, message);
}*/

/* Отправляет message на все устройства из списка подключенных devices */
/*void TCP::sendToAll(QString message) const  {
    qDebug() << "sending " + message + " to all...";
    foreach (Device device, *Devices::getDevices())  {
        send(device.getSocket(), message);
    }
}*/

/* private метод для отправки сообщения на непосредственно socket */
void TCP::send(QTcpSocket *socket, QString message) const  {
    if (socket != nullptr)  {
        QByteArray  arrBlock;
        QDataStream out(&arrBlock, QIODevice::WriteOnly);
        out << quint16(0) << message;
        out.device()->seek(0);
        out << quint16(arrBlock.size() - sizeof(quint16));
        socket->write(arrBlock);
    }   else    {
        qWarning() << "Sending error. Device socket isn't initialized.";
    }

}

/* приватный метод для инициализации таймера */
void TCP::initTimer()   {
    qDebug() << "timer initializing...";
    timer = new QTimer();
    timer->setInterval((int) 1000 / frequency);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotAskAll()));
}

/* Вызывает циклически вызываемую функцию опроса всех устройств */
void TCP::askAll(bool value)  {
    if (timer == nullptr)   {
        initTimer();
    }
    if (value)  {
        qDebug() << "asking loop has been started";
        timer->start();
    }   else    {
        qDebug() << "asking loop has been stoped";
        timer->stop();
    }
}

/* сеттер частоты опроса */
void TCP::setFrequency(float value)   {
    qDebug() << "asking frequency has been changed to " + QString::number(frequency);
    frequency = value;
    if (timer != nullptr)
        timer->setInterval((int) 1000 / frequency);
}

/* геттер частоты опроса */
float TCP::getFrequency() const {
    return frequency;
}

/* При подключении нового устройства будет вызван данный обработчик */
void TCP::slotNewConnection()   {
    qDebug() << "new connection";
    QTcpSocket* pClientSocket = server->nextPendingConnection();
    connect(pClientSocket, SIGNAL(disconnected()), pClientSocket, SLOT(slotDisconnected()));
    connect(pClientSocket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
}

/* При отключении устройства будет вызван данный обработчик. Функция определяет какое устройство было отключено и
 * удаляет его из списка устройств, а так же выдает соответствующее сообщение */
/*void TCP::slotDisconnected()    {
    qDebug() << "device disconnected";
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    foreach (Device device, *Devices::getDevices())  {
        if (device.getSocket() == pClientSocket)    {
            Devices::remove(device.getName());
            qDebug() << "device " + device.getName() + " removed from devices list";
        }
    }
}*/

/* Обработчик получения данных от устройств. Определяет корректность полученных данных и, если они корректны, то
 * определяет есть ли данное устройство в списке подключенных. Если такое уже подключено, то просто меняет его состояние.
 * Если нет, то так же добавляет его в список.*/
/*void TCP::slotReadClient()
{
    qDebug() << "incoming message";
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    //in.setVersion(QDataStream::Qt_4_2);
    quint16 blockSize = 0;
    QByteArray bytes;

    for (;;) {
        if (!blockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint16)) {
                break;
            }
            in >> blockSize;
        }

        if (pClientSocket->bytesAvailable() < blockSize) {
            break;
        }
        in >> bytes;
    }

    qDebug() << "received: " + bytes;

    //сообщение должно иметь фиксированное кол-во символов и '#' в начале и в конце
    //тогда оно будет воспринято как сообщение от Device
    if (!bytes.isEmpty())   {
        if (bytes.startsWith('#') && bytes.endsWith('#') && bytes.length() == messageLength) {
            QString name = QString::number(bytes[1] + bytes[2] + bytes[3]);
            char state = bytes[4];
            if (Devices::getDevices()->contains(name))    {
                qDebug() << "message from " + name + "; state: " + state;
                Devices::getDevices()->take(name).setPowerState(state);
            }   else    {
                qDebug() << "new device detected. name: " + name + ", state: " + state;
                Devices::add(name, Device(pClientSocket, name, state));
            }
        }   else    {
            qWarning() << "incorrect message format";
        }
    }
}*/

/* слот, вызываемый таймером timer с частотой frequency, и опрашивающий (отправляющий Device::MESSAGE_ASK)
 *  все устройства */
/*void TCP::slotAskAll()  {
    sendToAll(Device::MESSAGE_ASK);
}*/
