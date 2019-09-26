#ifndef WEATHERAPI_H
#define WEATHERAPI_H

#include <QObject>
#include <QMap>
#include <QNetworkReply>

class QNetworkRequest;

/*!
 * \brief WeatherApi - класс, реализующий модуль доступа к сервису openweathermap.org
 *
 * Стандартный путь доступа к Api - http://api.openweathermap.org/data/2.5
 * Для работы с Api требуется добавить в конец endpoint'а ключ appid со значением ApiKey,
 * предоставляемым сервисом после регистрации в личном кабинете на сайте openweathermap.org
 */
class WeatherApi : public QObject
{
    Q_OBJECT
public:
    explicit WeatherApi(QObject *parent = nullptr);

    void setHost(const QString &host);
    void setBasePath(const QString &basePath);
    void setApiKey(const QString &key);

    void addHeader(QString key, QString val);

    // API {
    /*!
     * \brief getWeatherData - функция получения данных погоды в указанном в параметре городе. Результат будет передан через сигнал weatherDataChanged
     * \param townName - навзание города. Список поддерживаемых городов с названиями можно узнать на сайте сервиса по адресу http://bulk.openweathermap.org/sample/city.list.json.gz
     */
    void getWeatherData(const QString townName);
    // }

signals:
    /*!
     * \brief weatherDataChanged - сигнал о получении погодных данных от сервиса
     * \param error - номер ошибки. В случае успешного обновления данных будет равняться QNetworkReply::NetworkError::NoError
     * \param errorStr - текст ошибки
     * \param jsonData - погодные данные в формате json, полученные от сервиса. В случае ошибки будет пустая строка.
     */
    void weatherDataChanged(QNetworkReply::NetworkError error, QString errorStr, QString jsonData);

private:
    void onWeatherDataReceived(QNetworkReply *reply);
    /*!
     * \brief executeRequest - функция для отправки запроса сервису.
     * \param request - объект с телом запроса
     * \param callbackFunction - callback-функция, которая будет вызвана после получения ответа от сервера либо в случае ошибки выполнения запроса.
     * \param QNetworkReply* - в качестве параметра callback-функции выступает указатель на объект с ответом.
     */
    void executeRequest(QNetworkRequest request, void (WeatherApi::*callbackFunction)(QNetworkReply *));

    QString _host;
    QString _basePath;
    QString _apiKey;

    QMap<QString, QString> _headers;
};

#endif // WEATHERAPI_H
