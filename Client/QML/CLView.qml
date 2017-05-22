import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0

Rectangle {
    width: parent.width / 3
    height: parent.height
    border.color: "black"
    border.width: 1
    color: "#aaf9af"

    ColumnLayout {
    id: card
    width: parent.width

//        Label { text: id }
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
            }
                anchors.top: src.bottom
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
            }
                anchors.top: tar.bottom
        }
        RowLayout {
            Button {
                id: deleteButton

                text: qsTr("Delete")
                height: parent.height - 6
                width: 100
                background: Rectangle {
                    color: pullButton.down ? '#7fb5b5' : '#c7d0cc'
                }

                enabled:  !(stateHolder.sync || stateHolder.learn)

                //ToolTip.visible: stateHolder.synchronizationInProgress
                //ToolTip.text: qsTr("you cannot delete card when synchronization is in progress")

                onClicked: {
                    controller.deleteCard(id);
                }
            }

            Button {
                id: editButton

                text: qsTr("Edit")
                height: parent.height - 6
                width: 100
                background: Rectangle {
                    color: pullButton.down ? '#7fb5b5' : '#c7d0cc'
                }
                enabled:  !(stateHolder.sync || stateHolder.learn)

                //ToolTip.visible: stateHolder.synchronizationInProgress
                //ToolTip.text: qsTr("you cannot edit card when synchronization is in progress")
                onClicked: {
                    controller.editCard(id);
                }
            }

        }
    }
}
