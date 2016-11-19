import QtQuick 2.0

import QtQuick.Layouts 1.0


ColumnLayout {
    width: parent.width

    Text {
        id: src

        Layout.fillWidth: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        text: source
    }

    Text {
        id: tar

        Layout.fillWidth: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        text: target
        wrapMode: Text.WordWrap
        anchors.top: src.bottom
    }

    Text {
        id: exmpl

        Layout.fillWidth: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        text: examples

        wrapMode: Text.WordWrap
        anchors.top: tar.bottom
    }
    Rectangle {
        width: parent.width
        height: 1
        color: "black"
    }
}
