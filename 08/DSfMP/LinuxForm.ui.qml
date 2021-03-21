import QtQuick 2.14
import QtQuick.Controls 2.2
import Qt.labs.qmlmodels 1.0

Page {
    id: page
    width: 600
    height: 400
    anchors.fill: parent
    property alias addButton: addButton
    property alias familyEdit: familyEdit
    property alias nameEdit: nameEdit

    title: qsTr("Linux distributions database")

    Item {
        id: addRow
        height: addButton.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        anchors.topMargin: 10
        Button {
            id: addButton
            text: "+"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.rightMargin: 0
        }
        Item {
            id: addFields
            anchors.verticalCenter: addButton.verticalCenter
            anchors.right: addButton.left
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.left: parent.left
            anchors.rightMargin: 10
            anchors.leftMargin: 0

            TextField {
                id: nameEdit
                text: ""
                anchors.left: parent.left
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: 5
                anchors.leftMargin: 0
                color: "white"
            }
            TextField {
                id: familyEdit
                text: ""
                anchors.left: parent.horizontalCenter
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.leftMargin: 5
                anchors.rightMargin: 0
                color: "white"
            }
        }
    }

    LinuxView {
        id: linuxView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: addRow.bottom
        anchors.bottom: parent.bottom
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.bottomMargin: 10
        anchors.topMargin: 10
        model: linuxModel
    }
}
