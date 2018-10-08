#include "requestmanager.h"

const QString RequestManager::URL_VIDEO_SYSTEM = "http://videoserver.com";

RequestManager::RequestManager()
{

}

void RequestManager::sendGETRequest(QString url, QString requestStr) {
    qDebug() << "sending GET request to " << url;
    QNetworkRequest request(url);
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(slotGetResponse(QNetworkReply*)));
    manager->get(QNetworkRequest(requestStr));
}

void RequestManager::slotGetResponse(QNetworkReply *reply)  {
    qDebug() << reply;
}

