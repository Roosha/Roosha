import QtQuick 2.0

import ".."

CardViewBase {
    implicitHeight: source.height + targets.height + examples.height

    Column {
        Text {
            id: source
            text: model.card.source
        }

        Text {
            id: targets
            text: model.card.target
        }

        Text {
            id: examples
            text: model.card.examples
        }
    }
}
