import QtQuick 2.0

import ".."

UserInputBase {
    implicitHeight: text.height

    Text {
        id: text
        text: "Some status should be here"
    }
}
