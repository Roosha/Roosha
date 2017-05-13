import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

import Roosha.controllers.translation 1.0

Rectangle {
    id: root
    width: 400; height: 200;
    color: "#fffddf"

    property bool closeByTimer: true

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: true
        onPositionChanged: {
            root.closeByTimer = false;
        }

        ColumnLayout{
            anchors.fill: parent
            spacing: 2

            Rectangle {
                Layout.minimumHeight: 30
                Layout.fillWidth: true
                color: "transparent"

                Text {
                    id: sourceElement

                    anchors.fill: parent

                    font.bold: true
                    font.pixelSize: 22
                    font.family: "Helvetica"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    text: trans[0].source
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ListView {
                    id: translationList

                    anchors.fill: parent

                    boundsBehavior: Flickable.StopAtBounds
                    maximumFlickVelocity: 500
                    orientation: ListView.Vertical

                    clip: true
                    focus: true

                    highlight: Component{
                        id: hightlightComponent
                        Rectangle {
                            width: translationList.currentItem.width
                            height: translationList.currentItem.height
                            y: translationList.currentItem.y
                            color: "#ebfad8"
                        }
                    }
                    highlightFollowsCurrentItem: false

                    model: trans
                    delegate: TranslationView {}
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.minimumHeight: 20
                color: "transparent"

                Row {
                    anchors.fill: parent
                    spacing: 5
                    anchors.leftMargin: 2

                    Button {
                        id: closeButton

                        width: 70
                        height: parent.height-2


                        font.family: "Helvetica"
                        text: qsTr("⮾Close")

                        onClicked: {
                            TranslationController.closeWindow(self);
                        }
                    }
                    Button {
                        id: createButton

                        width: 70
                        height: parent.height-2


                        font.family: "Helvetica"
                        text: qsTr("🗋Create")
                        enabled: !stateHolder.synchronizationInProgress
                        //ToolTip.visible: stateHolder.synchronizationInProgress
                        //ToolTip.text: qsTr("you cannot create card when synchronization is in progress")
                        onClicked: {
                            TranslationController.createCard(self, translationList.currentIndex);
                        }
                    }
                }
            }
        }
    }


    Timer {
        interval: 10000 // TODO: get from configuration
        running: true
        onTriggered: {
            if(root.closeByTimer) {
                TranslationController.closeWindow(self);
            }
        }
    }
}
