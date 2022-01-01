import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5

Window {
    id: root
    width: 400
    height: 300
    ColumnLayout {
        spacing: 3
        anchors.fill: parent
        ColumnLayout {
            Layout.margins: 10
            Layout.fillWidth: true
            Label {
                Layout.fillWidth: true
                text: "Area in millimeters:"
            }
            TextField {
                id: area_mm
                Layout.fillWidth: true
                text: GreenPoints.area_mm
            }
            Label {
                Layout.fillWidth: true
                text: "Area in pixels:"
            }
            TextField {
                id: area_pix
                Layout.fillWidth: true
                text: GreenPoints.area_pixels
            }
        }
        RowLayout {
            Layout.margins: 10
            Layout.fillWidth: true
            Button {
                Layout.fillWidth: true
                text: "OK"
                onClicked: {
                    GreenPoints.area_pixels = area_pix.text
                    GreenPoints.area_mm = area_mm.text
                    root.close()
                }
            }
            Button {
                Layout.fillWidth: true
                text: "Cancel"
                onClicked: root.close()
            }
        }
    }
}
