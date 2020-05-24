import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.1
import Dencrypter 1.0

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

    Dencrypter {
        id: dencrypter
        url: fileDialog.fileUrl
    }

    Text {
        id: keyText
        text: dencrypter.key
        visible: false
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
            ListElement {
                text: qsTr("Plain text")
                encrypt: "cat"
                decrypt: "cat"
                keylen: 16
            }
            ListElement {
                text: qsTr("AES128")
                encrypt: "main-128"
                decrypt: "main-128d"
                keylen: 128
            }
            ListElement {
                text: qsTr("AES192")
                encrypt: "main-192"
                decrypt: "main-192d"
                keylen: 192
            }
            ListElement {
                text: qsTr("AES256")
                encrypt: "main-256"
                decrypt: "main-256d"
                keylen: 256
            }
        }
    }

    TextField {
        id: textField
        text: hexKeyCheck.checked ? dencrypter.keyHex : dencrypter.key
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: combobox.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10

        Connections {
            target: textField
            onEditingFinished: {
                if (hexKeyCheck.checked)
                    dencrypter.keyHex = textField.text
                else
                    dencrypter.key = textField.text
            }
        }
    }

    Row {
        id: row
        height: 40
        spacing: 10
        anchors.top: textField.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10

        CheckBox {
            id: hexKeyCheck
            text: qsTr("HEX Key")
        }

        Button {
            id: generateButton
            text: qsTr("Generate Key")
        }

        Button {
            id: loadKeyButton
            text: qsTr("Load")
        }

        Button {
            id: saveKeyButton
            text: qsTr("Save")
        }

        Text {
            height: parent.height
            text: keyText.text.length * 8
            verticalAlignment: Text.AlignVCenter
            font.pixelSize: 20
        }
    }

    Row {
        id: row2
        height: 40
        spacing: 10
        anchors.top: textField.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 10

        CheckBox {
            id: hexDataCheck
            text: qsTr("HEX Data")
        }

        Button {
            id: loadDataButton
            text: qsTr("Load")
        }

        Button {
            id: saveDataButton
            text: qsTr("Save")
        }

        Button {
            id: encryptButton
            text: qsTr("Encrypt")
        }

        Button {
            id: decryptButton
            text: qsTr("Decrypt")
        }
    }

    Label {
        id: statusLine
        text: qsTr("App ready")
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
    }

    TextArea {
        id: textArea
        text: hexDataCheck.checked ? dencrypter.dataHex : dencrypter.data
        wrapMode: TextEdit.Wrap
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.top: row.bottom
        anchors.topMargin: 10
        anchors.bottom: statusLine.top
        anchors.bottomMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10

        Connections {
            target: textArea
            onEditingFinished: {
                if (hexDataCheck.checked)
                    dencrypter.dataHex = textArea.text
                else
                    dencrypter.data = textArea.text
            }
        }
    }

    Connections {
        target: dencrypter
        onKeyChanged: {
            var keylen = parseInt(model.get(combobox.currentIndex).keylen / 8)
            if (keyText.text.length !== keylen)
                textField.color = "red"
            else
                textField.color = "black"
        }
    }

    Connections {
        target: generateButton
        onClicked: {
            var keylen = model.get(combobox.currentIndex).keylen / 8
            dencrypter.generateKey(keylen)
            textField.color = "black"
        }
    }

    Connections {
        target: fileDialog
        onAccepted: {
            dencrypter.url = fileDialog.fileUrl
            console.log("User has selected " + fileDialog.fileUrl +
                        " and decrypter value is " + fileDialog.fileUrl)
            fileDialog.close()
            if (page.fileTarget == "loadData")
                if (dencrypter.loadData(hexDataCheck.checked))
                    statusLine.text = qsTr("Data file successfully loaded")
                else statusLine.text = qsTr("Failed to load data file")
            else if (page.fileTarget == "saveData")
                if (dencrypter.saveData(hexDataCheck.checked))
                    statusLine.text = qsTr("Data file successfully saved")
                else statusLine.text = qsTr("Failed to save data file")
            else if (page.fileTarget == "loadKey")
                if (dencrypter.loadKey(hexKeyCheck.checked))
                    statusLine.text = qsTr("Key file successfully loaded")
                else statusLine.text = qsTr("Failed to load key file")
            else if (page.fileTarget == "saveKey")
                if (dencrypter.saveKey(hexKeyCheck.checked))
                    statusLine.text = qsTr("Key file successfully saved")
                else statusLine.text = qsTr("Failed to save key file")
        }
    }

    Connections {
        target: loadDataButton
        onClicked: {
            page.fileTarget = "loadData"
            fileDialog.open()
        }
    }

    Connections {
        target: saveDataButton
        onClicked: {
            page.fileTarget = "saveData"
            fileDialog.open()
        }
    }

    Connections {
        target: loadKeyButton
        onClicked: {
            page.fileTarget = "loadKey"
            fileDialog.open()
        }
    }

    Connections {
        target: saveKeyButton
        onClicked: {
            page.fileTarget = "saveKey"
            fileDialog.open()
        }
    }

    Connections {
        target: encryptButton
        onClicked: {
            if (dencrypter.run(model.get(combobox.currentIndex).encrypt))
                statusLine.text = qsTr("Successfully encrypted")
            else
                statusLine.text = qsTr("Failed to encrypt")
        }
    }

    Connections {
        target: decryptButton
        onClicked: {
            if (dencrypter.run(model.get(combobox.currentIndex).decrypt))
                statusLine.text = qsTr("Successfully decrypted")
            else
                statusLine.text = qsTr("Failed to decrypt")
        }
    }
}
