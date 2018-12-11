#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>
#include <QTimer>
#include "map.h"


/*
 *данный класс реализует мэнеджер отправки http запросов и передачи в связанные слоты get ответов
 */
class RequestManager : public QObject
{
    Q_OBJECT
private:
    QTimer *timer;
    int frequency = 2;

protected:
    QNetworkAccessManager *manager;

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
    virtual void slotGetResponse(QNetworkReply *reply);
    void slotRequestForVideoData();

signals:
    void signalProcessData(QString data);
};

QString makeGET(QString url, QString request = "");

#endif // REQUESTMANAGER_H
