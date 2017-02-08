import QtQuick 2.0

import ".."

UserInputBase {
    implicitHeight: textInput.height

    TextInput {
        id: textInput

        text: "initial text"
        onAccepted: inputCompleted();
    }
}
