import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
    id: root
    width: 400; height: 200;
    color: "#fff8c2"


    Rectangle {
        width: parent.width
        height: parent.height

        Rectangle {
            id: src
            width: parent.width
            height: 40
            border.color: "black"
            border.width: 1

            TextInput {
                id: src_in
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: parent.width; height: parent.height
                text: trans.source
            }


        }
        Rectangle {
            id: tar
            width: parent.width
            height: 40
            border.color: "black"
            border.width: 1

            TextInput {
                id: tar_in
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: parent.width; height: parent.height
                text: trans.target
            }
            anchors.top: src.bottom
        }
        Rectangle {
            id: exmpl
            width: parent.width
            height: 40
            border.color: "black"
            border.width: 1

            TextInput {
                id: exmpl_in
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: parent.width; height: parent.height
                text: trans.examples
            }
            anchors.top: tar.bottom
        }
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
            width: 100

            background: Rectangle {
                color: closeButton.down ? '#7fb5b5' : '#c7d0cc'
            }

            onClicked: {
                controller.closeWindow();
            }
        }

        Button {
            id: saveButton

            text: qsTr("Save Card")
            height: parent.height - 6
            width: 100

            background: Rectangle {
                color: saveButton.down ? '#7fb5b5' : '#c7d0cc'
            }
            onClicked: {
                controller.saveCard(src_in.text, tar_in.text, exmpl_in.text);
            }
        }
    }
}
