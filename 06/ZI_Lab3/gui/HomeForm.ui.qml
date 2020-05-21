import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.1
import FileIO 1.0

Page {
    id: page
    width: 600
    height: 400
    property string fileTarget: ""
    anchors.fill: parent

    title: qsTr("Home")

    FileDialog {
        id: fileDialog
        title: "Please choose a file"
        visible: false
        selectMultiple: false
        selectFolder: false
        selectExisting: false
    }

    FileIOQML {
        id: fileio
    }

    ComboBox {
        id: combobox
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5
        textRole: "text"
        model: ListModel {
            id: model
            ListElement { text: qsTr("Plain text"); encrypt: "cat"; decrypt: "cat" }
            ListElement { text: qsTr("RSA"); encrypt: "rsae.py"; decrypt: "rsad.py" }
        }
    }

    Item {
        id: column
        width: parent.width / 2 - 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        anchors.top: combobox.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5

        Row {
            id: row
            height: 40
            spacing: 10
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0

            Button {
                id: load_button1
                text: qsTr("Load")
            }

            Button {
                id: save_button1
                text: qsTr("Save")
            }

            Button {
                id: encrypt_button
                text: qsTr("Encrypt")
            }
        }

        TextArea {
            id: textInput
            text: qsTr("Text Input")
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.top: row.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            font.pixelSize: 12
        }
    }

    Item {
        id: column1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        width: parent.width / 2 - 10
        anchors.right: parent.right
        anchors.rightMargin: 5
        anchors.top: combobox.bottom
        anchors.topMargin: 5

        Row {
            id: row1
            height: 40
            spacing: 10
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0

            Button {
                id: load_button2
                text: qsTr("Load")
            }

            Button {
                id: save_button2
                text: qsTr("Save")
            }

            Button {
                id: decrypt_button
                text: qsTr("Decrypt")
            }
        }

        TextArea {
            id: textInput1
            text: qsTr("Text Input")
            anchors.top: row1.bottom
            anchors.topMargin: 10
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            font.pixelSize: 12
        }
    }

    Connections {
        target: fileDialog
        onAccepted: {
            console.log("User has selected " + fileDialog.fileUrl)
            fileDialog.close()
            if (page.fileTarget == "load_button1") {
                fileio.url = fileDialog.fileUrl
                textInput.text = fileio.load()
            } else if (page.fileTarget == "save_button1") {
                fileio.url = fileDialog.fileUrl
                fileio.save(textInput.text)
            } else if (page.fileTarget == "load_button2") {
                fileio.url = fileDialog.fileUrl
                textInput1.text = fileio.load()
            } else if (page.fileTarget == "save_button2") {
                fileio.url = fileDialog.fileUrl
                fileio.save(textInput1.text)
            }
        }
    }

    Connections {
        target: load_button1
        onClicked: {
            page.fileTarget = "load_button1"
            fileDialog.open()
        }
    }

    Connections {
        target: load_button2
        onClicked: {
            page.fileTarget = "load_button2"
            fileDialog.open()
        }
    }

    Connections {
        target: encrypt_button
        onClicked: {
            textInput1.text = fileio.run(model.get(combobox.currentIndex).encrypt, textInput.text)
        }
    }

    Connections {
        target: save_button1
        onClicked: {
            page.fileTarget = "save_button1"
            fileDialog.open()
        }
    }

    Connections {
        target: save_button2
        onClicked: {
            page.fileTarget = "save_button2"
            fileDialog.open()
        }
    }

    Connections {
        target: decrypt_button
        onClicked: {
            textInput.text = fileio.run(model.get(combobox.currentIndex).decrypt, textInput1.text)
        }
    }
}
