import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

Slider {
    id:control

    property string color: 'black'
    property string colorTheme: control.enabled ? color : 'black'
    property bool showValue: true

    handle: Rectangle {
        id: sliderHandle
        x: control.leftPadding  + control.visualPosition * (control.availableWidth - width)
        y: control.topPadding + control.availableHeight / 2 - height / 2

        height: width
        width: control.height * 2

        radius: width
        color: control.pressed ? Qt.darker("white", 1.2) : "white"
        border{

            width: 1
            color: control.pressed ? Qt.lighter(colorTheme,1.2)
                                                  : Qt.lighter(colorTheme,1.5)
            Behavior on width{
               NumberAnimation {
                   duration: 100;
               }
           }
        }

        ToolTip {
            id: valuePreviewer

            parent: sliderHandle
            visible: control.showValue? pressed : false
            delay: 100
            timeout: 0

            contentItem: Text{
                topPadding: -8;
                bottomPadding: -8;
                text: value.toPrecision(4)
                font.pixelSize: 13
            }

            opacity: .8

            background: Rectangle{
                color: "Transparent"
                radius: width/2
            }
        }
    }
}

