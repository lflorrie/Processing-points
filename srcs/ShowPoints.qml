import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.5
import myextension 1.0
Window {
    id: root
    width: 800
    height: 600
    MouseArea {
        anchors.fill: parent
        onReleased: {
            liveImageItem.visible = true
            image.visible = false
            liveImageItem.setImage(GreenPoints.contour)
            root.title = "Contours"
        }
        onPressed: {
            liveImageItem.visible = false
            image.visible = true
//            image.source = "file:///" + GreenPoints.fileNameMaxGP
            image.source = GreenPoints.fileNameMaxGP
            root.title = "Original"
        }

    }
    ImageItem {
        id: image
        anchors.fill: parent
        source: GreenPoints.fileNameMaxGP
//        source: "file:///" + GreenPoints.fileNameMaxGP
    }
    ImageItem {
      id: liveImageItem
      anchors.fill: parent
    }
}
