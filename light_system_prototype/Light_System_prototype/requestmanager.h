#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDebug>

class RequestManager : public QObject
{
    Q_OBJECT
public:
    static const QString URL_VIDEO_SYSTEM;



    RequestManager();
    void sendGETRequest(QString url, QString requestStr);

public slots:
    void slotGetResponse(QNetworkReply *reply);

};

QString makeGET(QString url, QString request = "");

#endif // REQUESTMANAGER_H
