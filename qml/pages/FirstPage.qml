import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    palette.colorScheme: Theme.LightOnDark

    Rectangle {
        id: solidBackground
        anchors.fill: parent
        gradient: Gradient {
            GradientStop {position: 0.0; color: "#07a3f8"}
            GradientStop {position: 1.0; color: "#2055f8"}
        }
    }

    SilicaFlickable {
        anchors.fill: parent

        contentHeight: parent.height

        PageHeader {
            title: qsTr("Санкт-Петербург")
        }

        IconButton {
            id: actionButton

            anchors.centerIn: parent
            width: 250
            height: 250

            Image {
                id: customIcon

                anchors.fill: parent
                source: "qrc:///icons/custom/weather.png?" + (actionButton.pressed
                                                              ? Theme.highlightColor
                                                              : Theme.primaryColor)

                NumberAnimation on scale {
                    duration: 50
                    running: actionButton.pressed
                    from: 1.0; to: 0.9
                }

                NumberAnimation on scale {
                    duration: 50
                    running: !actionButton.pressed
                    from: 0.9; to: 1.0
                }
            }

            onClicked: pageStack.push(Qt.resolvedUrl("SecondPage.qml"));
        }

        Label {
            anchors.top: parent.top
            anchors.topMargin: parent.height*0.25
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: Theme.fontSizeMedium
            text: "Узнать погоду"
        }

    }
}
