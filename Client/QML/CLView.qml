import QtQuick 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4


ColumnLayout {
    id : card
    width: parent.width

    Label { text: id }
    Label {
        text: qsTr("source: ")
        width: 50
    }

    Text {
        id: src
        Layout.fillWidth: true
        text: source
        wrapMode: Text.WordWrap
    }

    Label {
        text: qsTr("target: ")
        width: 50
    }

    Text {
        id: tar
        Layout.fillWidth: true
        text: target
        wrapMode: Text.WordWrap
    }

    Label {
        text: qsTr("examples: ")
        width: 50
    }

    Text {
        id: exmpl
        Layout.fillWidth: true
        text: examples
        wrapMode: Text.WordWrap
    }
}
