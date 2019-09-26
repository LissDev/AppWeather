#ifndef WEATHERREFRESHER_H
#define WEATHERREFRESHER_H

#include <QObject>
#include <QNetworkReply>

class WeatherApi;

/*!
 * \brief WeatherRefresher - класс доступа к данным погоды, полученным от ресурса openweathermap.org
 */
class WeatherRefresher : public QObject
{
    Q_OBJECT
public:
    explicit WeatherRefresher(QObject *parent = nullptr);

    Q_PROPERTY(qint32 temp          MEMBER _temp)
    Q_PROPERTY(QString sunrise      MEMBER _sunrise)
    Q_PROPERTY(QString sunset       MEMBER _sunset)
    Q_PROPERTY(qint32 windSpeed     MEMBER _windSpeed)
    Q_PROPERTY(qreal pressure       MEMBER _pressure)
    Q_PROPERTY(qint32 humidity      MEMBER _humidity)
    Q_PROPERTY(qint32 clouds        MEMBER _clouds)

    /*!
     *\brief startRefreshProcess - функция запуска процесса обновления данных погоды в Санкт-Петербурге.
     * \todo Добавить в качестве параметров данные о городе, для получения данных погоды конкретного города.
     */
    Q_INVOKABLE void startRefreshProcess();

signals:
    /*!
     * \brief weatherDataChanged - сигнал об изменении данных погоды.
     */
    void weatherDataChanged();
    /*!
     * \brief receiveWeatherDataFailed - сигнал об ошибки обновления погодных данных.
     * \param error - тест ошибки
     */
    void receiveWeatherDataFailed(QString error);

private slots:
    void onWeatherDataReceived(QNetworkReply::NetworkError error, QString errorStr, QString jsonData);

private:
    qint32      _temp;      // Температура в градусах Цельсия
    QString     _sunrise;   // Время рассвета "hh:mm"
    QString     _sunset;    // Время заката "hh:mm"
    qint32      _windSpeed; // Скорость места в м/с
    qreal       _pressure;  // Атмосферное давление в мм рт.ст.
    qint32      _humidity;  // Влажность в %
    qint32      _clouds;    // Облачность в %

    WeatherApi *_weatherApi; // Модуль доступа к Api openweathermap.org
};

#endif // WEATHERREFRESHER_H
