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
        id: keyPairField
        text: hexKeyPairCheck.checked ? signer.keyPairHex : signer.keyPair
        width: parent.width / 2 - 15
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 10

        Connections {
            target: keyPairField
            onEditingFinished: {
                if (hexKeyPairCheck.checked)
                    signer.keyPairHex = keyPairField.text
                else
                    signer.keyPair = keyPairField.text
            }
        }
    }

    TextField {
        id: signatureField
        text: hexSignatureCheck.checked ? signer.signatureHex : signer.signature
        width: parent.width / 2 - 15
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.right: parent.right
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
        anchors.top: keyPairField.bottom
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 10

        CheckBox {
            id: hexKeyPairCheck
            text: qsTr("HEX Key pair")
        }

        CheckBox {
            id: hexSignatureCheck
            text: qsTr("HEX Signature")
        }

        Button {
            id: generateButton
            text: qsTr("Generate keys")
        }

        Button {
            id: loadKeyPairButton
            text: qsTr("Load key pair")
        }

        Button {
            id: saveKeyPairButton
            text: qsTr("Save key pair")
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

    Row {
        id: row2
        height: 40
        spacing: 10
        anchors.top: keyPairField.bottom
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
            else if (page.fileTarget == "loadKeyPair")
                if (signer.loadKeyPair(hexKeyPairCheck.checked))
                    statusLine.text = qsTr("Public key file successfully loaded")
                else statusLine.text = qsTr("Failed to load public key file")
            else if (page.fileTarget == "saveKeyPair")
                if (signer.saveKeyPair(hexKeyPairCheck.checked))
                    statusLine.text = qsTr("Public key file successfully saved")
                else statusLine.text = qsTr("Failed to save public key file")
            else if (page.fileTarget == "loadSignature")
                if (signer.loadSignature(hexSignatureCheck.checked))
                    statusLine.text = qsTr("Private key file successfully loaded")
                else statusLine.text = qsTr("Failed to load private key file")
            else if (page.fileTarget == "saveSignature")
                if (signer.saveSignature(hexSignatureCheck.checked))
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
        target: loadKeyPairButton
        onClicked: {
            page.fileTarget = "loadKeyPair"
            fileDialog.open()
        }
    }

    Connections {
        target: saveKeyPairButton
        onClicked: {
            page.fileTarget = "saveKeyPair"
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
            if (signer.check("check"))
                statusLine.text = qsTr("Successfully checked")
            else
                statusLine.text = qsTr("Failed to check")
        }
    }
}
