import QtQuick 2.14

LinuxForm {
    Connections {
        target: addButton
        onClicked: linuxModel.append({"name": nameEdit.text, "family": familyEdit.text})
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
