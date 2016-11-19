import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
    id: root
    width: 400; height: 200;
    color: "#fff8c2"
    border.color: "black"
    border.width: 1
    Text {
        id: sourceElement

        width: parent.width
        anchors.top: parent.top
        height: parent.height / 6

        font.bold: true
        font.pixelSize: 20
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter

        text: trans[0].source
    }

    Rectangle {
        anchors.top: sourceElement.bottom
        width: parent.width

        border.color: "black"
        border.width: 1
        height: parent.height * 2 / 3
        color: "#aaf9af"

        ListView {

            width: parent.width
            height: parent.height
            opacity: 1
            boundsBehavior: Flickable.DragAndOvershootBounds
            orientation: ListView.Vertical
            keyNavigationWraps: true
            //keyNavigationEnabled: true
            clip:true
            focus: true
            highlight: Rectangle { color: "#72f678" }
            model: trans
            delegate: TranslationView {}
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
                controller.closeWindow(identificator);
            }
        }

        Button {
            id: createButton

            text: qsTr("Create Card")
            height: parent.height - 6
            width: 100

            background: Rectangle {
                color: createButton.down ? '#7fb5b5' : '#c7d0cc'
            }
            onClicked: {
                controller.createCard(identificator);
            }
        }

//        Button {
//            id: createLaterButton

//            text: "Create later"
//            height: parent.height - 6
//            width: 100

//            background: Rectangle {
//                color: createLaterButton.down ? '#7fb5b5' : '#c7d0cc'
//            }
//        }
    }
}
