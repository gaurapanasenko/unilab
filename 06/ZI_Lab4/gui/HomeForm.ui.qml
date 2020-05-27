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

    TextField {
        id: pubKeyField
        text: hexPubKeyCheck.checked ? dencrypter.pubKeyHex : dencrypter.pubKey
        width: parent.width / 2 - 15
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10

        Connections {
            target: pubKeyField
            onEditingFinished: {
                if (hexPubKeyCheck.checked)
                    dencrypter.pubKeyHex = pubKeyField.text
                else
                    dencrypter.pubKey = pubKeyField.text
            }
        }
    }

    TextField {
        id: privKeyField
        text: hexPrivKeyCheck.checked ? dencrypter.privKeyHex : dencrypter.privKey
        width: parent.width / 2 - 15
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        Connections {
            target: privKeyField
            onEditingFinished: {
                if (hexPrivKeyCheck.checked)
                    dencrypter.privKeyHex = privKeyField.text
                else
                    dencrypter.privKey = privKeyField.text
            }
        }
    }

    Row {
        id: row
        height: 40
        spacing: 10
        anchors.top: pubKeyField.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10

        CheckBox {
            id: hexPubKeyCheck
            text: qsTr("HEX Pub Key")
        }

        CheckBox {
            id: hexPrivKeyCheck
            text: qsTr("HEX Priv Key")
        }

        Button {
            id: generateButton
            text: qsTr("Generate Keys")
        }

        Button {
            id: loadPubKeyButton
            text: qsTr("Load Pub")
        }

        Button {
            id: savePubKeyButton
            text: qsTr("Save Pub")
        }

        Button {
            id: loadPrivKeyButton
            text: qsTr("Load Priv")
        }

        Button {
            id: savePrivKeyButton
            text: qsTr("Save Priv")
        }
    }

    Row {
        id: row2
        height: 40
        spacing: 10
        anchors.top: pubKeyField.bottom
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
        target: generateButton
        onClicked: {
            if (dencrypter.generateKey("keygen"))
                statusLine.text = qsTr("Keys successfully generated")
            else statusLine.text = qsTr("Failed to generate keys")
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
            else if (page.fileTarget == "loadPubKey")
                if (dencrypter.loadPubKey(hexPubKeyCheck.checked))
                    statusLine.text = qsTr("Public key file successfully loaded")
                else statusLine.text = qsTr("Failed to load public key file")
            else if (page.fileTarget == "savePubKey")
                if (dencrypter.savePubKey(hexPubKeyCheck.checked))
                    statusLine.text = qsTr("Public key file successfully saved")
                else statusLine.text = qsTr("Failed to save public key file")
            else if (page.fileTarget == "loadPrivKey")
                if (dencrypter.loadPrivKey(hexPrivKeyCheck.checked))
                    statusLine.text = qsTr("Private key file successfully loaded")
                else statusLine.text = qsTr("Failed to load private key file")
            else if (page.fileTarget == "savePrivKey")
                if (dencrypter.savePrivKey(hexPrivKeyCheck.checked))
                    statusLine.text = qsTr("Private key file successfully saved")
                else statusLine.text = qsTr("Failed to save private key file")
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
        target: loadPubKeyButton
        onClicked: {
            page.fileTarget = "loadPubKey"
            fileDialog.open()
        }
    }

    Connections {
        target: savePubKeyButton
        onClicked: {
            page.fileTarget = "savePubKey"
            fileDialog.open()
        }
    }

    Connections {
        target: loadPrivKeyButton
        onClicked: {
            page.fileTarget = "loadPrivKey"
            fileDialog.open()
        }
    }

    Connections {
        target: savePrivKeyButton
        onClicked: {
            page.fileTarget = "savePrivKey"
            fileDialog.open()
        }
    }

    Connections {
        target: encryptButton
        onClicked: {
            if (dencrypter.encrypt("encrypt"))
                statusLine.text = qsTr("Successfully encrypted")
            else
                statusLine.text = qsTr("Failed to encrypt")
        }
    }

    Connections {
        target: decryptButton
        onClicked: {
            if (dencrypter.decrypt("decrypt"))
                statusLine.text = qsTr("Successfully decrypted")
            else
                statusLine.text = qsTr("Failed to decrypt")
        }
    }
}
