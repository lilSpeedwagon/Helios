#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QTimer>
#include "map.h"

class RequestManager : public QObject
{
    Q_OBJECT
private:
    QNetworkAccessManager *manager;
    QTimer *timer;
    int frequency = 2;

public:
    static const QString URL_VIDEO_SYSTEM;
    static const QString REQUEST_VIOEO_SYSTEM;

    RequestManager();
    void sendGETRequest(QString url, QString requestStr = "");
    void initTimer();
    void startTimer();
    void stopTimer();
    int getFrequency() const;
    void setFrequency(int freq);
    QString parseResponceStr(QString const& data);

    static QString getNetworkAdress();

public slots:
    void slotGetResponse(QNetworkReply *reply);
    void slotRequestForVideoData();

signals:
    void signalProcessData(QString data);
};

QString makeGET(QString url, QString request = "");

#endif // REQUESTMANAGER_H
