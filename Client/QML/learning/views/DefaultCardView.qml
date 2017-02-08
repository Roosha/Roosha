import QtQuick 2.0

import ".."

CardViewBase {
    implicitHeight: source.height + targets.height + examples.height

    Column {
        Text {
            id: source
            text: card.source
        }

        Text {
            id: targets
            text: card.target
        }

        Text {
            id: examples
            text: card.examples
        }
    }
}
