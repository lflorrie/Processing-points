import QtQuick 2.3
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

Item {
    id: root
    width: 480
    height: 320
    property alias header: name.text
    property alias text: text_data.text

    Rectangle {
        id: rect
        border.color: "black"
        color: "white"
        width: parent.width
        height: parent.height
        ScrollView {
            id: scrollView // This is just an ID of scroll view
            width: parent.width // This is the width of the scroll view window
            height: parent.height // This is the height of the scroll view window
            contentWidth: name.width + text_data.width
            contentHeight: name.height + text_data.height
            clip: true

            Column {
                width: parent.width
                height: parent.height

                TextArea {
                    id: name
                    color: "black"
                    enabled: false
                    font.pixelSize: 25
                    height: font.pixelSize + 30
                }
                TextArea {
                    id: text_data
                    color: "black"
                    enabled: false
                    font.pixelSize: 14
                }
            }
        }
    }
}
