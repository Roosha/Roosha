import QtQuick 2.0

import QtQuick.Layouts 1.0

Rectangle {
    color: "transparent"
    width: parent.width
    height: textLay.height

    ColumnLayout {
        id: textLay
        width: parent.width

        Text {
            id: tar

            Layout.fillWidth: true

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.pixelSize: 20
            font.family: "Helvetica"

            text: target == "" ? "no target" : target
            wrapMode: Text.WordWrap
        }

        Text {
            id: exmpl

            Layout.fillWidth: true

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            font.pixelSize: 12
            font.family: "Helvetica"

            text: examples
            wrapMode: Text.WordWrap
        }

        MouseArea {
            anchors.fill: parent
            onClicked: translationList.currentIndex = index
            onDoubleClicked: {
                controller.createCard(self, index)
            }
        }
    }

    Rectangle{
        width: parent.width
        height: 1
        color: "black"
        anchors.bottom: parent.bottom
    }
}
