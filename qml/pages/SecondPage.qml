import QtQuick 2.0
import Sailfish.Silica 1.0
import io.backend 1.0

Page {
    id: page

    palette.colorScheme: Theme.LightOnDark

    function showWeatherInfo(isVisible) {
        mainWeatherInfo.visible = isVisible;
        adWeatherInfo.visible = isVisible;

        errorMessage.visible = !isVisible;
        if(!isVisible && (errorMessage.text.length === 0)) {
            errorMessage.text = "Данные о погоде загружаются...";
        }
    }

    function showErrorMessage(error) {
        errorMessage.visible = true;
        errorMessage.text = error;
    }

    WeatherRefresher {
        id: weatherRefresher

        onWeatherDataChanged: {
            errorMessage.text = "";
            progressPanel.open = false;
            progressPanelOpener.stop();
            showWeatherInfo(true);

            tempLabel.text = temp + "º";
            adWeatherData.set(0, {"value": sunrise});
            adWeatherData.set(1, {"value": sunset});
            adWeatherData.set(2, {"value": windSpeed + " м/с"});
            adWeatherData.set(3, {"value": pressure.toFixed(2) + " мм рт.ст."});
            adWeatherData.set(4, {"value": humidity + "%"});
            adWeatherData.set(5, {"value": clouds + "%"});
            refreshTimer.start();
        }

        onReceiveWeatherDataFailed: {
            errorMessage.text = error;
            showWeatherInfo(false);
            refreshTimer.start();
        }
    }

    Rectangle {
        id: solidBackground
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {position: 0.0; color: "#07a3f8"}
            GradientStop {position: 1.0; color: "#2055f8"}
        }
    }

    Item {
        anchors.top: parent.top
        anchors.topMargin: parent.height*0.25
        anchors.left: parent.left
        width: parent.width

        Label {
            id: errorMessage
            anchors.horizontalCenter: parent.horizontalCenter
        }

        Label {
            id: errorMessageTimer

            visible: errorMessage.visible && refreshTimer.running
            anchors.top: errorMessage.bottom
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }

    Column {
        id: mainWeatherInfo

        Label {
            id: townLabel
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: Theme.fontSizeExtraLarge
            text: "Санкт-Петербург"
        }

        Label {
            id: tempLabel
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: 250
        }

        GlassItem {
            id: effect
            height: Theme.paddingLarge
            width: page.width
            falloffRadius: 0.2
            radius: 0.2
            color: palette.highlightColor
            cache: false
        }
    }

    SilicaListView {
        id: adWeatherInfo

        anchors.top: mainWeatherInfo.bottom
        anchors.topMargin: Theme.paddingLarge
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        width: parent.width

        spacing: 10

        model: ListModel {
            id: adWeatherData

            ListElement {
                name: "Рассвет: "
                value: ""
            }
            ListElement {
                name: "Закат: "
                value: ""
            }
            ListElement {
                name: "Ветер: "
                value: ""
            }
            ListElement {
                name: "Давление: "
                value: ""
            }
            ListElement {
                name: "Влажность: "
                value: ""
            }
            ListElement {
                name: "Облачность: "
                value: ""
            }
        }

        delegate: Item {
            width: ListView.view.width
            height: adInfoLabel.height

            Label {
                id: adInfoLabel

                anchors.horizontalCenter: parent.horizontalCenter
                height: text.contentHeight
                text: name + value
            }
        }

        Label {
            id: timeToRefresh

            visible: refreshTimer.running
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            font.pixelSize: Theme.fontSizeExtraSmall
            property int value: 0

            Timer {
                id: refreshTimer

                interval: 1000
                running: page.status === PageStatus.Active
                triggeredOnStart: true
                repeat: true

                onTriggered: {
                    if(timeToRefresh.value == 0) {
                        stop();
                        timeToRefresh.value = 60;
                        progressPanelOpener.start();
                        weatherRefresher.startRefreshProcess();
                    }

                    timeToRefresh.text = "До следующего обновления осталось " +  timeToRefresh.value-- + " секунд";
                    errorMessageTimer.text = timeToRefresh.text;
                }
            }
        }
    }

    DockedPanel {
        id: progressPanel

        width:parent.width
        height: Theme.itemSizeExtraLarge

        dock: Dock.Bottom

        Timer {
            id: progressPanelOpener
            interval: 1000
            onTriggered: progressPanel.open = true
        }

        ProgressCircle {
            id: progressCircle

            anchors.centerIn: parent

            NumberAnimation on value {
                from: 0
                to: 1
                duration: 1000
                running: progressPanel.expanded
                loops: Animation.Infinite
            }
        }
    }

    onStatusChanged: {
        if(status === PageStatus.Activating) {
            showWeatherInfo(false);
        }
    }
}
