import QtQuick 2.0
import roosha.learning 1.0

Item {
    property CardLearningModel model: null

    /// before emitting the signal, user input should be checked according to model.card and
    /// settings stored in model.inputModel
    ///
    /// If input is empty or skipped, model.difficulty should be set to Difficulty.SKIPPED
    /// If input is non-empty but wrong, model.difficulty should be set to Difficulty.FAILED
    /// Otherwise, model.difficulty should be set to Difficulty.UNKNOWN (this is default value for the
    /// property, so setting it is optional as long as it was not manually changed).
    signal inputCompleted
}
