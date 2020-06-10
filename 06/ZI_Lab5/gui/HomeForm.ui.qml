import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Dialogs 1.1
import Signer 1.0

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

    Signer {
        id: signer
        url: fileDialog.fileUrl
    }

    TextField {
        id: pubKeyField
        text: hexPubKeyCheck.checked ? signer.pubKeyHex : signer.pubKey
        width: parent.width / 2 - 15
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10

        Connections {
            target: pubKeyField
            onEditingFinished: {
                if (hexPubKeyCheck.checked)
                    signer.pubKeyHex = pubKeyField.text
                else
                    signer.pubKey = pubKeyField.text
            }
        }
    }

    TextField {
        id: privKeyField
        text: hexPrivKeyCheck.checked ? signer.privKeyHex : signer.privKey
        width: parent.width / 2 - 15
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        Connections {
            target: privKeyField
            onEditingFinished: {
                if (hexPrivKeyCheck.checked)
                    signer.privKeyHex = privKeyField.text
                else
                    signer.privKey = privKeyField.text
            }
        }
    }

    Row {
        id: row3
        height: 40
        spacing: 10
        anchors.top: privKeyField.bottom
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 10

        CheckBox {
            id: hexSignatureCheck
            text: qsTr("HEX Sign")
        }

        Button {
            id: loadSignatureButton
            text: qsTr("Load signature")
        }

        Button {
            id: saveSignatureButton
            text: qsTr("Save signature")
        }
    }

    TextField {
        id: signatureField
        text: hexSignatureCheck.checked ? signer.signatureHex : signer.signature
        //width: parent.width / 2 - 15
        anchors.top: pubKeyField.bottom
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: row3.left
        anchors.rightMargin: 10

        Connections {
            target: signatureField
            onEditingFinished: {
                if (hexSignatureCheck.checked)
                    signer.signatureHex = signatureField.text
                else
                    signer.signature = signatureField.text
            }
        }
    }

    Row {
        id: row
        height: 40
        spacing: 10
        anchors.top: signatureField.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10

        CheckBox {
            id: hexPubKeyCheck
            text: qsTr("HEX Pub key")
        }

        CheckBox {
            id: hexPrivKeyCheck
            text: qsTr("HEX Priv key")
        }

        Button {
            id: generateButton
            text: qsTr("Generate keys")
        }

        Button {
            id: loadPubKeyButton
            text: qsTr("Load pub key")
        }

        Button {
            id: savePubKeyButton
            text: qsTr("Save pub key")
        }

        Button {
            id: loadPrivKeyButton
            text: qsTr("Load priv key")
        }

        Button {
            id: savePrivKeyButton
            text: qsTr("Save priv key")
        }
    }

    Row {
        id: row2
        height: 40
        spacing: 10
        anchors.top: signatureField.bottom
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
            id: signButton
            text: qsTr("Sign")
        }

        Button {
            id: checkButton
            text: qsTr("Check")
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
        text: hexDataCheck.checked ? signer.dataHex : signer.data
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
                    signer.dataHex = textArea.text
                else
                    signer.data = textArea.text
            }
        }
    }

    Connections {
        target: signer
        onSignStatusChanged: {
            if (signer.signStatus)
                statusLine.text = qsTr("Valid signature")
            else
                statusLine.text = qsTr("Invalid signature")
        }
    }

    Connections {
        target: generateButton
        onClicked: {
            if (signer.generateKey("keygen"))
                statusLine.text = qsTr("Keys successfully generated")
            else statusLine.text = qsTr("Failed to generate keys")
        }
    }

    Connections {
        target: fileDialog
        onAccepted: {
            signer.url = fileDialog.fileUrl
            console.log("User has selected " + fileDialog.fileUrl +
                        " and checker value is " + fileDialog.fileUrl)
            fileDialog.close()
            if (page.fileTarget == "loadData")
                if (signer.loadData(hexDataCheck.checked))
                    statusLine.text = qsTr("Data file successfully loaded")
                else statusLine.text = qsTr("Failed to load data file")
            else if (page.fileTarget == "saveData")
                if (signer.saveData(hexDataCheck.checked))
                    statusLine.text = qsTr("Data file successfully saved")
                else statusLine.text = qsTr("Failed to save data file")
            else if (page.fileTarget == "loadPubKey")
                if (signer.loadPubKey(hexPubKeyCheck.checked))
                    statusLine.text = qsTr("Public key file successfully loaded")
                else statusLine.text = qsTr("Failed to load public key file")
            else if (page.fileTarget == "savePubKey")
                if (signer.savePubKey(hexPubKeyCheck.checked))
                    statusLine.text = qsTr("Public key file successfully saved")
                else statusLine.text = qsTr("Failed to save public key file")
            else if (page.fileTarget == "loadPrivKey")
                if (signer.loadPrivKey(hexPrivKeyCheck.checked))
                    statusLine.text = qsTr("Private key file successfully loaded")
                else statusLine.text = qsTr("Failed to load private key file")
            else if (page.fileTarget == "savePrivKey")
                if (signer.savePrivKey(hexPrivKeyCheck.checked))
                    statusLine.text = qsTr("Private key file successfully saved")
                else statusLine.text = qsTr("Failed to save private key file")
            else if (page.fileTarget == "loadSignature")
                if (signer.loadSignature(hexSignatureCheck.checked))
                    statusLine.text = qsTr("Signature file successfully loaded")
                else statusLine.text = qsTr("Failed to load signature file")
            else if (page.fileTarget == "saveSignature")
                if (signer.saveSignature(hexSignatureCheck.checked))
                    statusLine.text = qsTr("Signature file successfully saved")
                else statusLine.text = qsTr("Failed to save signature file")
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
        target: loadSignatureButton
        onClicked: {
            page.fileTarget = "loadSignature"
            fileDialog.open()
        }
    }

    Connections {
        target: saveSignatureButton
        onClicked: {
            page.fileTarget = "saveSignature"
            fileDialog.open()
        }
    }

    Connections {
        target: signButton
        onClicked: {
            if (signer.sign("sign"))
                statusLine.text = qsTr("Successfully signed")
            else
                statusLine.text = qsTr("Failed to sign")
        }
    }

    Connections {
        target: checkButton
        onClicked: {
            signer.check("check")
            /*if ()
                statusLine.text = qsTr("Successfully checked")
            else
                statusLine.text = qsTr("Failed to check")*/
        }
    }
}
