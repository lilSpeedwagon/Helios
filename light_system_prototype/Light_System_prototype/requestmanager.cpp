#include "requestmanager.h"

const QString RequestManager::URL_VIDEO_SYSTEM = "http://videoserver.com";
const QString RequestManager::REQUEST_VIOEO_SYSTEM = "";


RequestManager::RequestManager()
{
    qDebug() << "-----------------------------";
    qDebug() << "Initializing request manager...";
    manager = new QNetworkAccessManager(this);
    initTimer();
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(slotGetResponse(QNetworkReply*)));
    connect(this, SIGNAL(signalProcessData(QString)), Map::getMapPtr(), SLOT(slotProcessData(QString)));
    qDebug() << "Done.";
}

void RequestManager::sendGETRequest(QString url, QString requestStr) {
    qDebug() << "sending GET request " << url + requestStr;
    manager->get(QNetworkRequest(QUrl(url + requestStr)));
    //manager->get(QNetworkRequest(makeGET(url, requestStr)));
}

void RequestManager::slotGetResponse(QNetworkReply *reply)  {
    qDebug() << "-----------------------------";
    qDebug() << "getting responce...";
    if (reply->error() == QNetworkReply::NoError) {
        QString data = QString::fromUtf8( reply->readAll() );
        qDebug() << "data: ";
        qDebug() << data;

        emit signalProcessData(parseResponceStr(data));
    } else {
        qDebug() << "error: " << reply->errorString();
    }
    reply->deleteLater();
    qDebug() << "Done.";
}

void RequestManager::initTimer()   {
    qDebug() << "initializing request manager timer...";
    timer = new QTimer();
    timer->setInterval((int) 1000 / frequency);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotRequestForVideoData()));
    qDebug() << "Done.";
}

void RequestManager::startTimer()   {
    qDebug() << "starting timer...";
    timer->start();
    qDebug() << "Done.";
}

void RequestManager::stopTimer()    {
    qDebug() << "stopping map timer...";
    timer->stop();
    qDebug() << "Done.";
}

QString RequestManager::parseResponceStr(QString const& data)   {
    return data;
}


QString RequestManager::getNetworkAdress()  {
    QString networkName;

    return networkName;
}

void RequestManager::slotRequestForVideoData()  {
    sendGETRequest(URL_VIDEO_SYSTEM, REQUEST_VIOEO_SYSTEM);
}

QString makeGET(QString url, QString request)   {
    QString GETStr;
    GETStr.append("GET ");
    GETStr.append(url);
    GETStr.append(request);
    GETStr.append(" HTTP/1.0\r\n");
    GETStr.append("Host: ");
    GETStr.append(url);
    GETStr.append("\r\n");
    //GETStr.append("User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:16.0) Gecko/20100101 Firefox/16.0\r\n");
    GETStr.append("\r\n");

    return GETStr;
}
