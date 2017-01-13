import QtQuick 2.7
import QtQuick.Layouts 1.0
import QtQuick.Controls 1.4


ColumnLayout {
    width: parent.width - 20
    height: parent.height * 5 / 6 - 10
    anchors.bottom: parent.bottom - parent.height / 6
    anchors.horizontalCenter: parent.horizontalCenter

    function getSource() {
        return src_in.text;
    }

    function getTarget() {
        return tar_in.text;
    }


    function getExample() {
        return exmpl_in.text;
    }
    function getId() {
        return trans.id;
    }


    ColumnLayout {
        id: src
        width: parent.width
        height: 40
        spacing: 5

        Label {
            text: qsTr("source: ")
            width: parent.width / 5

        }

        TextField {
            Layout.fillWidth: true
            id: src_in
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            text: trans.source
        }
        Layout.fillHeight: true
    }

    ColumnLayout {
        id: tar
        width: parent.width
        height: 40
        spacing: 5

        Label {
            text: qsTr("target: ")
            width: parent.width / 5

        }

        TextField {
            id: tar_in
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width; height: parent.height
            text: trans.target
        }
        Layout.fillHeight: true
    }

    ColumnLayout {
        id: exmpl
        width: parent.width
        height: 80
        spacing: 5

        Label {
            text: qsTr("example: ")
            width: parent.width / 5

        }

        TextArea {
            id: exmpl_in
            Layout.fillWidth: true
            Layout.fillHeight: true
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            width: parent.width; height: parent.height
            text: trans.examples
        }
        Layout.fillHeight: true
    }
}
