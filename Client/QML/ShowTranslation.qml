import QtQuick 2.7
import QtQuick.Controls 2.0


Rectangle {
    id: root
    width: 400; height: 200;
    color: "#fff8c2"
    border.color: "black"
    border.width: 1

    property bool closeByTimer: true

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
            id: translationList

            width: parent.width
            height: parent.height

            boundsBehavior: Flickable.StopAtBounds
            maximumFlickVelocity: 500
            orientation: ListView.Vertical

            clip:true
            focus: true

            highlight: Component{
                id: hightlightComponent
                Rectangle {
                    width: translationList.currentItem.width
                    height: translationList.currentItem.height
                    y: translationList.currentItem.y-5
                    color: "#ffff88"
                }
            }
            highlightFollowsCurrentItem: false

            model: trans
            delegate: TranslationView {}
        }
    }



    Timer {
        interval: 10000 // TODO: get from configuration
        running: true
        onTriggered: {
            if(root.closeByTimer) {
                controller.closeWindow(self);
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onPositionChanged: {
            root.closeByTimer = false;
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
                    controller.closeWindow(self);
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
                    controller.createCard(self, translationList.currentIndex);
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
}
