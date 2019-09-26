#include "weatherrefresher.h"

#include <QTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QByteArray>

#include "weatherapi.h"

WeatherRefresher::WeatherRefresher(QObject *parent) :
    QObject(parent),
    _temp(0),
    _windSpeed(0),
    _pressure(0),
    _humidity(0),
    _clouds(0)
{
    _weatherApi = new WeatherApi(this);

    connect(_weatherApi, &WeatherApi::weatherDataChanged, this, &WeatherRefresher::onWeatherDataReceived);
}

void WeatherRefresher::startRefreshProcess()
{
    _weatherApi->getWeatherData("Saint Petersburg");
}

void WeatherRefresher::onWeatherDataReceived(QNetworkReply::NetworkError error, QString errorStr, QString jsonData)
{
    if(error == QNetworkReply::NoError) {
        QJsonObject jsonObj = QJsonDocument::fromJson(QByteArray(jsonData.toStdString().c_str())).object();

        if(jsonObj.isEmpty()) {
            emit receiveWeatherDataFailed("Ошибка соединения");
            return;
        }

        if(jsonObj.contains("main") && jsonObj["main"].isObject()) {
            QJsonObject mainObj = jsonObj["main"].toObject();
            if(mainObj.contains("temp")) {
                _temp = qRound(mainObj["temp"].toDouble() - 273.15); // Перевод температуры из Кельвин в градусы
            }
            if(mainObj.contains("pressure")) {
                _pressure = mainObj["pressure"].toInt()*0.750062; // Перевод давления из гПа в мм рт.ст.
            }
            if(mainObj.contains("humidity")) {
                _humidity = mainObj["humidity"].toInt();
            }
        }

        if(jsonObj.contains("wind") && jsonObj["wind"].isObject()) {
            QJsonObject windObj = jsonObj["wind"].toObject();
            if(windObj.contains("speed")) {
                _windSpeed = windObj["speed"].toInt();
            }
        }

        if(jsonObj.contains("clouds") && jsonObj["clouds"].isObject()) {
            QJsonObject cloudObj = jsonObj["clouds"].toObject();
            if(cloudObj.contains("all")) {
                _clouds = cloudObj["all"].toInt();
            }
        }

        if(jsonObj.contains("sys") && jsonObj["sys"].isObject()) {
            QJsonObject sysObj = jsonObj["sys"].toObject();

            // Данные от openweathermap.org приходят в формате unix
            if(sysObj.contains("sunrise")) {
                QDateTime dt_sunrise = QDateTime::fromTime_t(sysObj["sunrise"].toVariant().toUInt());
                _sunrise = dt_sunrise.toString("hh:mm");
            }
            if(sysObj.contains("sunset")) {
                QDateTime dt_sunset = QDateTime::fromTime_t(sysObj["sunset"].toVariant().toUInt());
                _sunset = dt_sunset.toString("hh:mm");
            }
        }

        emit weatherDataChanged();
    } else {
        emit receiveWeatherDataFailed("Ошибка соединения\n" + errorStr);
    }
}
