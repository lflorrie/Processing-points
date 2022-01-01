import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

ColumnLayout {
    id: col
    width: parent.width
    spacing: 3

    property alias text: text.text
    property alias label_text: label.text

    property alias but1_text: but.text
    property alias but2_text: but2.text
    property var but1_clicked
    property var but2_clicked
   Label {
        id: label
        Layout.fillWidth: true
    }
    RowLayout {
       TextField {
            id: text
            Layout.fillWidth: true
        }
        Button {
            id: but
            height: text.height
            Layout.fillWidth: true
            Layout.minimumWidth: 50
            Layout.minimumHeight: 30
            Layout.maximumWidth: 50
            text:"Find"
            onClicked: but1_clicked()
        }
        Button {
            id: but2
            height: parent.height
            Layout.minimumWidth: 50
            Layout.minimumHeight: 30
            Layout.maximumWidth: 50
            Layout.fillWidth: true
            onClicked: but2_clicked()
            text:"OK"
        }
    }
}
