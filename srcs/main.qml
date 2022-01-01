import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    title: qsTr("GreenPoints")
    color : "white"
    ColorAnimation on color { id: color1; to: "#e5e5ff"; duration: 1000 }
    Connections {
        target: GreenPoints

        function onSendToQml(text_stream) {
           txtout.text = text_stream
        }
    }

    function save_file() {
        GreenPoints.saveFile(text_experiment.text, input_save_file.text)
    }

    ColumnLayout {
        id: col
        width: parent.width
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        spacing: 3
        ColumnLayout {
            Layout.fillWidth: true
            Layout.margins: 10
            Label {
                 id: label
                 Layout.fillWidth: true
                 text: "Name of experiment: "
            }
            RowLayout {
                Layout.fillWidth: true
                TextField {
                    id: text_experiment
                    Layout.alignment: Qt.AlignLeft
                    Layout.fillWidth: true
                }
                Button {
                    text: "Setting"
                    Layout.alignment: Qt.AlignRight
                    Layout.minimumWidth: 106
                    onClicked: {
                        var component = Qt.createComponent("SettingsGreenPoints.qml")
                        var window    = component.createObject(root)
                        window.show()
                    }
                }
            }
        }
        RowInput {
            id: input_folder
            label_text: "Open folder with data:"
            but1_clicked: fileDialog.open
            but2_clicked: GreenPoints.receiveFromQml
            Layout.margins: 10
            text: GreenPoints.path
            onTextChanged: GreenPoints.path = input_folder.text
        }
        TextOutput {
            id: txtout
            header: text_experiment.text
            Layout.fillWidth: true
            Layout.fillHeight: true

            Layout.margins: 10
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
        RowInput {
            id: input_save_file
            but2_text: "Save"
            label_text: "Save file to: "
            text: GreenPoints.path_save
            but2_clicked: save_file
            but1_clicked: fileDialog2.open
            Layout.margins: 10
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
            input_folder.text = decodeURIComponent(t_path1)

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
        onAccepted: {
            console.log("You chose: " + fileDialog2.fileUrls)
            var t_path = fileDialog2.fileUrl.toString();
            t_path = t_path.replace(/^(file:\/{3})/,"");
            input_save_file.text = decodeURIComponent(t_path)
        }
        onRejected: {
            console.log("Canceled")
        }
    }

}
