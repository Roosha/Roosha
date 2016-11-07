import QtQuick 2.0

Rectangle {
    width: parent.width
    height: 60
    border.color: "black"
    border.width: 1

    Text {
        id: src
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width; height: parent.height / 3
        text: source
    }

    Text {
        id: tar
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width; height: parent.height / 3
        text: target
        anchors.top: src.bottom
    }

    Text {
        id: exmpl
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width; height: parent.height / 3
        text: examples
        anchors.top: tar.bottom
    }
}
