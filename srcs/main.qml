import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    id: root
    title: "GreenPoints"
    width: 1200
    height: 800
    visible: true

    property int borderWidthVal: 2
    property int layoutMarginsVal: 20
    property int pixelSize: 18


    Connections {
        target: GreenPoints

        function onSendToQml(text_stream) {
           txtout.text = text_stream
        }
    }

    SwipeView {
        id: mainSwipeVeiw
        anchors.fill: parent
        clip: true
        interactive: false
        Button {
            text: qsTr("Choice the folder")
            onClicked: {
                fileDialog.open()
            }
        }
        GridLayout {
            columns: 3
            MyButton {
                text: qsTr("Back")
                Layout.fillWidth: true
                Layout.margins: layoutMarginsVal
                borderWidth: borderWidthVal
                onClicked: {
                    mainSwipeVeiw.decrementCurrentIndex()
                }

            }
            TextOutput {
                id: txtout
                pixelSize: root.pixelSize
                header: textOfExperiment.text
                borderWidth: borderWidthVal
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.column: 0
                Layout.row: 1
                Layout.columnSpan: 2
                Layout.rowSpan: 2
//                anchors.fill: parent
                Layout.margins: layoutMarginsVal
                ProgressBar {
                    id: progress_bar
                    visible: false
                    from: 0
                    to: 100
                    value: GreenPoints.progress
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    Connections {
                        target: GreenPoints
                        function onProcessingChanged(progress) {
                            progress_bar.visible = true;
                                progress_bar.value = progress;
                            if (progress_bar.value === 100)
                                progress_bar.visible = false
                        }
                    }
                }
            }
            Rectangle {
                Layout.column: 2
                Layout.row: 1
                Layout.margins: layoutMarginsVal
                Layout.fillWidth: true
                Layout.fillHeight: true
                SwipeView {
                    id: swipeView
                    clip: true
                    interactive: false
                    anchors.fill: parent
                    GridLayout {
                    id: lay1
                    columns: 3

                    //
                    MyButton {
                        opacity: 1
                        Layout.column: 2
                        text: ""
                        borderWidth: borderWidthVal
//                        Layout.fillHeight: true
                        implicitWidth: 50
                        implicitHeight: 50
//                        Layout.margins: layoutMarginsVal
                        Layout.rowSpan: 5
                        onClicked: {
                            swipeView.incrementCurrentIndex()
                        }
                        Image {
                            id: imgArrowRightButton
                            source: "../resources/arrow_3.svg"
                            anchors.centerIn: parent
                            width: parent.width * 0.5
                            height: parent.width * 0.5
                        }
                    }
                    //

                    Label {
                        Layout.column: 0
                        text: qsTr("Name of experiment:")
                        font.pixelSize: pixelSize
                    }
                    MyTextField {
                        id: textOfExperiment
                        font.pixelSize: pixelSize
                        selectByMouse: true
                        borderWidth: borderWidthVal
                        placeholderText: qsTr("Enter name of experiment")
                        Layout.columnSpan: 1
                        Layout.fillWidth: true
                        Layout.margins: layoutMarginsVal
                    }
                    Label {
                        text: qsTr("Units of Area:")
                        font.pixelSize: pixelSize
                    }
                    MyTextField {
                        id: units
                        font.pixelSize: pixelSize
                        borderWidth: borderWidthVal
                        Layout.margins: layoutMarginsVal
                        selectByMouse: true
                        placeholderText: qsTr("Enter units of area")
                        text: GreenPoints.unitsArea
                        onTextChanged: {
                            GreenPoints.unitsArea = units.text
                        }

                        Layout.columnSpan: 1
                        Layout.fillWidth: true
                    }
                    Label {
                        text: qsTr("Pixels to mm (S):")
                        font.pixelSize: pixelSize
                    }
                    RowLayout {
                        MyTextField {
                            id: area_pix
                            font.pixelSize: pixelSize
                            borderWidth: borderWidthVal
                            Layout.margins: layoutMarginsVal
                            selectByMouse: true
                            placeholderText: qsTr("px")
                            text: GreenPoints.area_pixels
                            Layout.fillWidth: true
                            onTextChanged: {
                                GreenPoints.area_pixels = area_pix.text
                            }
                        }
                        MyTextField {
                            id: area_mm
                            font.pixelSize: pixelSize
                            borderWidth: borderWidthVal
                            Layout.margins: layoutMarginsVal
                            selectByMouse: true
                            placeholderText: qsTr("mm")
                            text: GreenPoints.area_mm
                            Layout.fillWidth: true
                            onTextChanged: {
                                GreenPoints.area_mm = area_mm.text
                            }
                        }
                    }

                    Label {
                        text: qsTr("Path to background:")
                        font.pixelSize: pixelSize
                    }
                    MyTextField {
                        id: backgroundPath
                        borderWidth: borderWidthVal
                        font.pixelSize: pixelSize
                        placeholderText: qsTr("Enter path to background:")
                        selectByMouse: true
                        Layout.columnSpan: 1
                        Layout.fillWidth: true
                        Layout.margins: layoutMarginsVal
                        text: GreenPoints.pathToBackground
                        enabled: backgroundCheckBox.checked
                        onTextChanged: {
                            GreenPoints.pathToBackground = backgroundPath.text
                        }
                        MyButton {
                            opacity: 1
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            onClicked: {
                                fileDialog3.open()
                            }
                            text: "^"
                            borderWidth: borderWidthVal
                            width: parent.width / 8
                        }
                    }
                    CheckBox {
                        id: showOnlyMaxResult
                        font.pixelSize: pixelSize
                        checked: GreenPoints.showOnlyMaxResult
                        onToggled: {
                            showButton.enabled = showOnlyMaxResult.checked
                            GreenPoints.showOnlyMaxResult = showOnlyMaxResult.checked
                            GreenPoints.receiveFromQml()
                        }
                        text: qsTr("Show only max result")
                        Layout.fillWidth: true
                        Layout.columnSpan: 1
                    }
                    CheckBox {
                        id: backgroundCheckBox
                        font.pixelSize: pixelSize

                        checked: GreenPoints.useBackgroundSubtraction
                        onToggled: {
                            backgroundPath.enabled = backgroundCheckBox.checked
                            GreenPoints.useBackgroundSubtraction = backgroundCheckBox.checked
                            GreenPoints.receiveFromQml()
                        }
                        text: qsTr("Subtraction background")
                        Layout.alignment: Qt.AlignRight
//                        Layout.fillWidth: true
                        Layout.columnSpan: 1

                    }
                }
                    GridLayout {
                    id: lay2
                    columns: 3
                        MyButton {
                            Layout.column: 0
                            text: ""
//                            Layout.fillHeight: true
                            borderWidth: borderWidthVal
                            Layout.margins: layoutMarginsVal
                            Layout.rowSpan: 3
                            implicitWidth: 50
                            implicitHeight: 50
                            onClicked: {
                                swipeView.decrementCurrentIndex()
                            }
                            Image {
                                source: "../resources/arrow_left_3.svg"
                                anchors.centerIn: parent
                                width: parent.width * 0.5
                                height: parent.width * 0.5
                            }
                        }
                        Label {
                            text: qsTr("Sensivity:")

                            font.pixelSize: pixelSize
                        }
                        MySlider {
                            id: sensivity
                            from: 0
                            to: 255
                            stepSize: 1
                            value: GreenPoints.sensivity
                            snapMode: Slider.SnapOnRelease
                            onPressedChanged: {
                                if (!pressed) {
                                    GreenPoints.sensivity = sensivity.value
                                    sensivity.enabled = false
                                    GreenPoints.receiveFromQml()
                                    sensivity.enabled = true
                                }
                            }
                            Layout.fillWidth: true
                        }
//                        Slider {
//                            Layout.fillWidth: true

//                        }
//                        Slider {
//                            Layout.fillWidth: true

//                        }
                    }
                }
            }

            PageIndicator {
                id: indicator
                count: swipeView.count
                currentIndex: swipeView.currentIndex
                Layout.column: 2
                Layout.columnSpan: 2
                Layout.alignment: (Qt.AlignCenter | Qt.AlignBottom)
                Layout.row: 0
                Layout.margins: layoutMarginsVal
            }
            RowLayout {
                MyButton {
                    borderWidth: borderWidthVal
                    Layout.column: 2
                    Layout.row: 2
                    Layout.margins: layoutMarginsVal
                    Layout.fillWidth: true
                    text: qsTr("Save")
                    onClicked: {
                        fileDialog2.open()
                    }
                }
                MyButton {
                    id: showButton
                    borderWidth: borderWidthVal
                    Layout.margins: layoutMarginsVal
                    Layout.fillWidth: true
                    text: qsTr("Show")
                    enabled: showOnlyMaxResult.checked
                    onClicked: {
                        var component = Qt.createComponent("ShowPoints.qml")
                        var window    = component.createObject(root)
                        window.show()
                    }
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: qsTr("Please choose a file")
        folder: GreenPoints.path.toString() === "" ? shortcuts.home : "file:///" + GreenPoints.path;
        selectFolder: true

        onAccepted: {
            var t_path1 = fileDialog.fileUrl.toString();
            t_path1 = t_path1.replace(/^(file:\/{3})/,"");

//            input_folder.text = decodeURIComponent(t_path1)
            GreenPoints.path = decodeURIComponent(t_path1)
            mainSwipeVeiw.incrementCurrentIndex()
            GreenPoints.receiveFromQml()
        }
        onRejected: {
            console.log("Canceled")
        }
    }
    FileDialog {
        id: fileDialog2
        title: qsTr("Please choose a file")
        folder: {
            folder = GreenPoints.path_save === "" ? shortcuts.home : "file:///" + GreenPoints.path_save;
        }
        selectExisting: false
        nameFilters: ["Text files(*.txt)", "All files (*)"]
//        name
        onAccepted: {
            console.log("You chose: " + fileDialog2.fileUrls)
            var t_path = fileDialog2.fileUrl.toString();
            t_path = t_path.replace(/^(file:\/{3})/,"");
//            input_save_file.text = decodeURIComponent(t_path)
            GreenPoints.saveFile(textOfExperiment.text, decodeURIComponent(t_path))
        }
        onRejected: {
            console.log("Canceled")
        }
    }
    FileDialog {
        id: fileDialog3
        title: qsTr("Please choose a file")
        folder: {
            folder = GreenPoints.path_save === "" ? shortcuts.home : "file:///" + GreenPoints.pathToBackground;
        }
        //selectExisting: false
        nameFilters: ["Text files(*.bmp)", "All files (*)"]
//        name
        onAccepted: {
            console.log("You chose: " + fileDialog3.fileUrls)
            var t_path = fileDialog3.fileUrl.toString();
            t_path = t_path.replace(/^(file:\/{3})/,"");
//            input_save_file.text = decodeURIComponent(t_path)
            GreenPoints.pathToBackground = decodeURIComponent(t_path);
            GreenPoints.receiveFromQml()
        }
        onRejected: {
            console.log("Canceled")
        }
    }
}
