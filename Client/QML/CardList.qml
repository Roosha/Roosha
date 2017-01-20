import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
    id: root
    width: 600; height: 300;
    color: "#fff8c2"
    border.color: "black"
    border.width: 1
    Text {
        id: mainWin

        width: parent.width
        anchors.top: parent.top
        height: parent.height / 6

        font.bold: true
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: qsTr("Your Cards:")
    }
    Rectangle {
        anchors.top: mainWin.bottom
        width: parent.width

        border.color: "black"
        border.width: 1
        height: parent.height * 2 / 3
        color: "#faee93"

        GridView {
            width: parent.width
            height: parent.height
            cellHeight: parent.height
            cellWidth: width / 3
            clip: true

            model: cards
            delegate: CLView {}
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
            anchors.left: parent.left
            anchors.leftMargin: 10
            background: Rectangle {
                color: closeButton.down ? '#7fb5b5' : '#c7d0cc'
            }

            onClicked: {
                controller.closeWindow();
            }
        }

        Button {
            id: createButton

            text: qsTr("Create Card")
            height: parent.height - 6
            width: 100
            anchors.right: parent.right
            anchors.rightMargin: 10
            background: Rectangle {
                color: createButton.down ? '#7fb5b5' : '#c7d0cc'
            }
            onClicked: {
                controller.createCard();
            }
            enabled: !stateHolder.sync
        }

        Button {
            id: pushButton

            text: qsTr("Push")
            height: parent.height - 6
            width: 100
            anchors.right: createButton.left
            anchors.rightMargin: 10
            background: Rectangle {
                color: pushButton.down ? '#7fb5b5' : '#c7d0cc'
            }
            onClicked: {
                controller.pushCards();
            }
            enabled: stateHolder.editionWindowOpen == 0

        }

        Button {
            id: pullButton

            text: qsTr("Pull")
            height: parent.height - 6
            width: 100
            anchors.right: pushButton.left
            anchors.rightMargin: 10
            background: Rectangle {
                color: pullButton.down ? '#7fb5b5' : '#c7d0cc'
            }
            onClicked: {
                controller.pullCards();
            }
            enabled: stateHolder.editionWindowOpen == 0
        }
    }
}
