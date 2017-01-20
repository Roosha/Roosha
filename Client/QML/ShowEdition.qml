import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
    id: root
    width: 400; height: 200;
    color: "#fff8c2"

    EditView {
        id: edit
    }


    Row {
        width: parent.width
        height: parent.height / 6
        anchors.bottom: parent.bottom

        leftPadding: 5
        rightPadding: 5
        topPadding: 3
        bottomPadding: 3

        spacing: 4

        Button {
            id: closeButton

            text: qsTr("Close")
            height: parent.height - 6

            background: Rectangle {
                color: closeButton.down ? '#7fb5b5' : '#c7d0cc'
            }
            width: 100
            anchors.left: parent.left
            anchors.leftMargin: 10

            onClicked: {
                controller.closeWindow(self);
            }
        }

        Button {
            id: saveButton

            text: qsTr("Save Card")
            height: parent.height - 6

            background: Rectangle {
                color: saveButton.down ? '#7fb5b5' : '#c7d0cc'
            }

            width: 100
            anchors.right: parent.right
            anchors.rightMargin: 10

            onClicked: {
                controller.saveCard(self, edit.getSource(), edit.getTarget(), edit.getExample(), edit.getId());
            }
        }
    }
}
