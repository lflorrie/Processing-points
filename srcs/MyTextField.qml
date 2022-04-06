import QtQuick 2.0
import QtQuick.Controls 2.12

TextField {
    id: units
    property int borderWidth: 1
    selectByMouse: true
    implicitWidth: 200
    implicitHeight: 50
    background: Rectangle {
        border.width: borderWidth
    }
}
