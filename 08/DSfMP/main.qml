import QtQuick 2.9
import QtQuick.Controls 2.2

ApplicationWindow {
    id: window
    width: 640
    height: 480
    visible: true
    //title: qsTr("Stack")

    header: ToolBar {
        id: toolbar
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u2190" : "\u2261"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }

        ComboBox {
            id: comboBox
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 10

            model: ["uk_UA", "en_US"]

            onCurrentTextChanged: {
                qmlTranslator.setTranslation(comboBox.currentText)
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.RightButton
        onClicked: drawer.open()
    }

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("PixyWorld")
                width: parent.width
                onClicked: {
                    stackView.push("PixyWorld.ui.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Linux distributions database")
                width: parent.width
                onClicked: {
                    stackView.push("Linux.qml", {"linuxModel": linuxModel})
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomeForm.ui.qml"
        anchors.fill: parent
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.5}D{i:4}
}
##^##*/
