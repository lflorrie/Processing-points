import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

ApplicationWindow {
    id: root
    title: "GreenPoints"
    width: 1000
    height: 800
    visible: true

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
            text: "Choice the folder"
            onClicked: {
                fileDialog.open()
            }
        }
        GridLayout {
            columns: 3

            Button {
                text: "Back"
                Layout.fillWidth: true
                Layout.margins: 10
                onClicked: {
                    mainSwipeVeiw.decrementCurrentIndex()
                }

            }
            TextOutput {
                id: txtout
                header: textOfExperiment.text
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.column: 0
                Layout.row: 1
                Layout.columnSpan: 2
                Layout.rowSpan: 2
//                anchors.fill: parent
                Layout.margins: 20
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
                Layout.margins: 10
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
                    Button {
                        Layout.column: 2
                        text: "->"
                        Layout.fillHeight: true
//                        Layout.margins: 10
                        Layout.rowSpan: 5
                        onClicked: {
                            swipeView.incrementCurrentIndex()
                        }
                    }
                    //

                    Label {
                        Layout.column: 0
                        text: "Name of experiment"
                    }
                    TextField {
                        id: textOfExperiment
                        placeholderText: "Enter name of experiment"
                        Layout.columnSpan: 1
                        Layout.fillWidth: true
                    }
                    Label {
                        text: "Units S"
                    }
                    TextField {
                        id: units
                        placeholderText: "Enter units of area"
                        text: GreenPoints.unitsArea
                        onTextChanged: {
                            GreenPoints.unitsArea = units.text
                        }

                        Layout.columnSpan: 1
                         Layout.fillWidth: true
                    }
                    Label {
                        text: "Pixels to mm (S):"
                    }
                    RowLayout {
                        TextField {
                            id: area_pix
                            placeholderText: "px"
                            text: GreenPoints.area_pixels
                            Layout.fillWidth: true
                            onTextChanged: {
                                GreenPoints.area_pixels = area_pix.text
                            }
                        }
                        TextField {
                            id: area_mm
                            placeholderText: "mm"
                            text: GreenPoints.area_mm
                            Layout.fillWidth: true
                            onTextChanged: {
                                GreenPoints.area_mm = area_mm.text
                            }
                        }
                    }

                    Label {
                        text: "Path to background:"
                    }
                    TextField {
                        id: backgroundPath
                        placeholderText: "Enter path to background:"
                        Layout.columnSpan: 1
                        Layout.fillWidth: true
                        text: GreenPoints.pathToBackground
                        enabled: backgroundCheckBox.checked
                        onTextChanged: {
                            GreenPoints.pathToBackground = backgroundPath.text
                        }
                        Button {
                            anchors.right: parent.right
                            anchors.top: parent.top
                            anchors.bottom: parent.bottom
                            onClicked: {
                                fileDialog3.open()
                            }
                            text: "^"
                            width: parent.width / 8
                        }
                    }
                    CheckBox {
                        id: showOnlyMaxResult
                        checked: GreenPoints.showOnlyMaxResult
                        onToggled: {
                            showButton.enabled = showOnlyMaxResult.checked
                            GreenPoints.showOnlyMaxResult = showOnlyMaxResult.checked
                            GreenPoints.receiveFromQml()
                        }
                        text: "Show only max result"
                        Layout.fillWidth: true
                        Layout.columnSpan: 1
                    }
                    CheckBox {
                        id: backgroundCheckBox
                        checked: GreenPoints.useBackgroundSubtraction
                        onToggled: {
                            backgroundPath.enabled = backgroundCheckBox.checked
                            GreenPoints.useBackgroundSubtraction = backgroundCheckBox.checked
                            GreenPoints.receiveFromQml()
                        }
                        text: "Subtraction backgound"
                        Layout.fillWidth: true
                        Layout.columnSpan: 1

                    }
                }
                    GridLayout {
                    id: lay2
                    columns: 2
                        Button {
                            Layout.column: 0
                            text: "<-"
                            Layout.fillHeight: true
                            Layout.margins: 10
                            Layout.rowSpan: 3
                            onClicked: {
                                swipeView.decrementCurrentIndex()
                            }
                        }
                        Slider {
                            id: sensivity
                            from: 0
                            to: 255
                            stepSize: 10
                            value: GreenPoints.sensivity
                            snapMode: Slider.SnapOnRelease
                            onPressedChanged: {
                                if (!pressed) {
                                    GreenPoints.sensivity = sensivity.value
                                    GreenPoints.receiveFromQml()
                                }
                            }
                            onPositionChanged: {
                                sensivityLabel.text = sensivity.value
//                                sensivityLabel.x = sensivity.position + visualPosition * sensivity.width
                            }
                            Layout.fillWidth: true
                            Label {
                                id: sensivityLabel
                                text: sensivity.value
                            }
                        }
                        Slider {
                            Layout.fillWidth: true

                        }
                        Slider {
                            Layout.fillWidth: true

                        }
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
                Layout.margins: 20
            }
            RowLayout {
                Button {
                    Layout.column: 2
                    Layout.row: 2
                    Layout.margins: 20
                    Layout.fillWidth: true
                    text: "Save"
                    onClicked: {
                        fileDialog2.open()
                    }
                }
                Button {
                    id: showButton
                    Layout.margins: 20
                    Layout.fillWidth: true
                    text: "Show"
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
        title: "Please choose a file"
        folder: GreenPoints.path === "" ? shortcuts.home : "file:///" + GreenPoints.path;
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
        title: "Please choose a file"
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
        title: "Please choose a file"
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
