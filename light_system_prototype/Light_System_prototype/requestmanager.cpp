#include "requestmanager.h"

const QString RequestManager::URL_VIDEO_SYSTEM = "http://videoserver.com";

RequestManager::RequestManager()
{

}

void RequestManager::sendGETRequest(QString url, QString requestStr) {
    qDebug() << "sending GET request to " << url;

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(slotGetResponse(QNetworkReply*)));
    manager->get(QNetworkRequest(makeGET(url, requestStr)));
}

void RequestManager::slotGetResponse(QNetworkReply *reply)  {
    qDebug() << reply;
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
