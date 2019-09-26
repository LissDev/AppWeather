#include "weatherapi.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonDocument>

WeatherApi::WeatherApi(QObject *parent) :
    QObject(parent),
    _host("http://api.openweathermap.org/data"),
    _basePath("/2.5"),
    _apiKey("d03ff26557dbb7aff0c3a73b281d86ff")
{
}

void WeatherApi::setBasePath(const QString &basePath)
{
    _basePath = basePath;
}

void WeatherApi::setHost(const QString &host)
{
    _host = host;
}

void WeatherApi::setApiKey(const QString &key)
{
    _apiKey = key;
}

void WeatherApi::addHeader(QString key, QString val)
{
    _headers.insert(key, val);
}

void WeatherApi::getWeatherData(const QString townName)
{
    QString url = _host + _basePath + "/weather?q=" + townName + "&appid=" + _apiKey;

    QNetworkRequest request = QNetworkRequest(QUrl(url));

    executeRequest(request, &WeatherApi::onWeatherDataReceived);
}

void WeatherApi::onWeatherDataReceived(QNetworkReply *reply)
{
    QNetworkReply::NetworkError errorType = reply->error();
    QString errorStr = reply->errorString();
    QByteArray response = reply->readAll();

    reply->deleteLater();

    emit weatherDataChanged(errorType, errorStr, QString(response));
}

void WeatherApi::executeRequest(QNetworkRequest request, void (WeatherApi::*callbackFunction)(QNetworkReply*))
{
    foreach(QString key, _headers.keys()) {
        request.setRawHeader(key.toStdString().c_str(), _headers.value(key).toStdString().c_str());
    }

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);

    connect(manager, &QNetworkAccessManager::finished, this, callbackFunction);
    connect(manager, &QNetworkAccessManager::finished, manager, &QNetworkAccessManager::deleteLater);

    manager->get(request);
}
