import QtQuick 2.0
import QtQuick.Controls 2.0

Button {
    id: myButton

//    property color colorDefault: "#86C0D6"
//    property color colorMouseOver: "#A9D8EA"
//    property color colorPressed: "#7495A1"
//    property color colorDisabled: "#7495A1"
//    property color colorBorder: "transparent"
    property color colorDefault: "white"
    property color colorMouseOver: "#C0C0C0"
    property color colorPressed: "#F3F3F3"
    property color colorDisabled: "#8D8D8D"
    property color colorBorder: "black"
    property int borderWidth: 2

    QtObject {
        id: internal
        property var dynamicColor: {
            if (!myButton.enabled) {
                colorDisabled
            } else if(myButton.down) {
               myButton.down ? colorPressed : colorDefault
            } else {
               myButton.hovered ? colorMouseOver : colorDefault
            }
        }
    }
    text: qsTr("Button")
    implicitWidth: 200
    implicitHeight: 50
    background: Rectangle {
        border.width: borderWidth
        border.color: colorBorder
        color: internal.dynamicColor
        radius: 0
    }
    contentItem: Item {
        Text {
            id: textButton
            text: myButton.text
            anchors.centerIn: parent
            font.pointSize: 13
        }
    }
}
