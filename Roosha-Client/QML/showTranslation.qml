import QtQuick 2.0
import QtQuick.Controls 2.0

Rectangle {
    id: root
    width: 200; height: 150;

    Text {
        id: textes
        width: parent.width; height: parent.height
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        text: trans[0].source

    }
    Button {
        id: button
        x: parent.width/2 - button.width/2
        y: parent.height/2 + 10
        width: 50
        height: 20
        text: qsTr("Button me")
    }

    ListView {
        width: 100; height: 100

        model: trans
        delegate: Rectangle {
            height: 20
            width: 100
            Text { text: source }
        }
    }

}
