#ifndef CALLMANAGER_H
#define CALLMANAGER_H

#include "requestmanager.h"
#include "device.h"
#include "devices.h"

class CallManager : public RequestManager
{
    Q_OBJECT
private:
    int deviceNumber = 0;
    int progress = 0;
    QString currentAdress;
    QString adress;

    bool callingForDevices = false;

    Devices *devices;
public:
    static const QString REQUEST_CALL;
    static const QString REQUEST_ON;
    static const QString REQUEST_OFF;
    static const QString HTTP;

    CallManager() = delete;
    CallManager(Devices *devices);
    void callDevices();
    void call(QString adress);
    void checkConnected(Device const& device);
    void sendToDevice(Device const& device, QString request);

    void setAdress(QString adress);
    QString getAdress() const;
    int getProgress() const;

public slots:
    void slotGetResponse(QNetworkReply *reply);
signals:
    void signalEndCalling();
};

#endif // CALLMANAGER_H
