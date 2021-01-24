import QtQuick 2.9
import QtQuick.Controls 2.2

Page {
    width: 600
    height: 400
    anchors.fill: parent

    title: qsTr("Page 2")

    Label {
        text: qsTr("You are on Page 2.")
        anchors.centerIn: parent
    }
}
