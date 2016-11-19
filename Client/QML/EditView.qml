import QtQuick 2.7

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
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width; height: parent.height
            text: trans.examples
        }
        anchors.top: tar.bottom
    }
}
