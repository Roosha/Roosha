import QtQuick 2.0

import ".."

CardViewBase {
    implicitHeight: examples.height + delimiter.height + targets.height

    Column {
        Text {
            id: examples
            text: card.examples
        }

        Text {
            id: delimiter
            text: "----"
        }

        Text {
            id: targets
            text: card.target
        }
    }
}
