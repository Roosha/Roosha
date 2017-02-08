import QtQuick 2.0
import roosha.data 1.0


Item {
    property Card card: null
    property var model: null

    Component.onCompleted: {
        console.log("CardViewBase.card: ", card);
        console.log("CardViewBase.model: ", model);
    }
}
