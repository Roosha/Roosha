import QtQuick 2.0
import roosha.learning 1.0


Item {
    property CardLearningModel model: null

    Component.onCompleted: {
        console.log("CardViewBase.model: ", model);
    }
}
