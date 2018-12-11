#include "callmanager.h"

const QString CallManager::REQUEST_CALL = "/?request=call";
const QString CallManager::REQUEST_ON = "/?request=on";
const QString CallManager::REQUEST_OFF = "/?request=off";
const QString CallManager::HTTP = "http://";

CallManager::CallManager(Devices *devices)
{
    this->devices = devices;
}

void CallManager::callDevices() {
    qDebug() << "calling devices...";
    callingForDevices = true;
    deviceNumber = 0;

    for (int i = 0; i < 255; i++)  {
        if (!callingForDevices)
            break;
        progress = i;
        currentAdress = adress + QString::number(i);
        call(HTTP + currentAdress);
    }

    currentAdress.clear();
    callingForDevices = false;
    emit signalEndCalling();
    qDebug() << "Done. " << deviceNumber << " devices found";
}

void CallManager::call(QString adress)    {
    sendGETRequest(adress, REQUEST_CALL);
}

void CallManager::sendToDevice(Device const& device, QString request) {
    currentAdress = device.getAdress();
    sendGETRequest(HTTP + currentAdress, request);
}

void CallManager::setAdress(QString adress) {
    this->adress = adress;
}

QString CallManager::getAdress() const  {
    return adress;
}

int CallManager::getProgress() const    {
    return progress;
}

void CallManager::slotGetResponse(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8( reply->readAll() );

        qDebug() << "Responce. Data: " << data;
        if (data == "DONE") {
            for (Device &device : devices->getDevices())   {
                if (device.getAdress() == currentAdress)  {
                    qDebug() << "Device " << data << " switched.";
                    device.changeState();
                }
            }
        }   else    {
            QString name = data.section(':', 0, 0);
            QString adress = data.section(':', 1, 1);
            for (Device &device : devices->getDevices())   {
                if (name == device.getName())  {
                    qDebug() << "Device " << data << " connected.";
                    device.setAdress(adress);
                    deviceNumber++;
                }
            }
        }


    } else {
        qDebug() << "responce error: " << reply->errorString();
    }
    reply->deleteLater();
}

void CallManager::checkConnected(Device const& device)   {
    sendToDevice(device, REQUEST_CALL);
}
