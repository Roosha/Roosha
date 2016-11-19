import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4

Rectangle {
    width: parent.width / 3
    height: parent.height
    border.color: "black"
    border.width: 1
    color: "#aaf9af"

    ColumnLayout {
    id: card
    width: parent.width

        //Label { text: id
        RowLayout {

            id: src
            width: parent.width
            Label {
                text: qsTr("source: ")
                width: 50
            }

            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: parent.width
                text: source
            }
        }
        RowLayout {
            id: tar
            width: parent.width
            Label {
                text: qsTr("target: ")
                width: 50
            }

            Text {

                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: parent.width
                text: target
                wrapMode: Text.WordWrap
                anchors.top: src.bottom
            }
        }

        RowLayout {
            id: exmpl
            width: parent.width
            Label {
                text: qsTr("examples: ")
                width: 50
            }

            Text {
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                width: parent.width
                text: examples

                wrapMode: Text.WordWrap
                anchors.top: tar.bottom
            }
        }
    }
}
