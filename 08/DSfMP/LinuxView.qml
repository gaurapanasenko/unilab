import QtQuick 2.14
import QtQuick.Controls 2.2
import Qt.labs.qmlmodels 1.0

ListView {
    id: view
    height: 300
    delegate: Item {
        id: row
        height: button.height
        x: 10
        width: view.width - 20

        Button {
            id: button
            text: "-"
            onClicked: view.model.remove(linux_id)
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.rightMargin: 0
        }
        Item {
            anchors.verticalCenter: button.verticalCenter
            anchors.right: button.left
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.rightMargin: 10
            anchors.leftMargin: 0

            TextField {
                id: nameEdit
                text: name
                anchors.left: parent.left
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: 5
                anchors.leftMargin: 0
                color: "white"
                onEditingFinished: linuxModel.update(linux_id, "name", text)
            }
            TextField {
                id: familyEdit
                text: family
                anchors.left: parent.horizontalCenter
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.leftMargin: 5
                anchors.rightMargin: 0
                color: "white"
                onEditingFinished: linuxModel.update(linux_id, "family", text)
            }
        }
    }
}
