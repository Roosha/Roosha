import QtQuick 2.0

import ".."
import roosha.learning 1.0

UserInputBase {
    implicitHeight: textInput.height

    TextInput {
        id: textInput

        text: "initial text"
        onAccepted: completeInput(text);
    }

    function completeInput(text) {
        console.log("input completed. Text: ", text);
        var source = model.card.source;
        if (!text) {
            model.difficultyRate = Difficulty.SKIPPED;
        } else if (!(model.inputModel.caseInsensitive ? text.toUpperCase() === source.toUpperCase() :
                                                        text === source)) {
            model.difficultyRate = Difficulty.FAILED;
        }

        console.log("caseInsensitive: ", model.inputModel.caseInsensitive);
        console.log('"', text, '".compareLocale("', source, '" === ',  text.toUpperCase() === source.toUpperCase());
        inputCompleted();
    }
}
