import QtQuick 2.0

Rectangle {
    width: parent.width
    height: 40

    border.color: "black"
    border.width: 1

    Text {
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width; height: parent.height
        text: source
    }
}
