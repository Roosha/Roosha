import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import roosha.learning 1.0
import roosha.data 1.0

Rectangle {
    id: root
    height: 500
    width: 550
    Component.onCompleted: onFirstCard();

    /// 0 -- initial,
    /// 1 -- front side,
    /// 2 -- back side after correct input(evaluating difficulty)
    /// 3 -- back side after incorrect input
    /// 4 -- nothing to show
    property int state: 0

    UserInputBase {
        Component.onCompleted: console.log("UserInputBase completed");
    }

    Column {
        anchors.fill: parent
        spacing: 10

        SwipeView {
            id: cardViewport

            height: parent.height - buttonPanel.height - 10
            width: parent.width

            interactive: false
        }

        // Bottom panel with 'Close' button and buttons for difficulty assessing
        Rectangle {
            id: buttonPanel

            height: easyButton.height
            width: parent.width

            Button {
                id: nextButton

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                text: qsTr("Next card")
                onClicked: onNextCard(null)
                enabled: root.state === 3
            }

            Button {
                id: easyButton

                anchors {
                    left: nextButton.right
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }

                text: qsTr("Easy")
                onClicked: onNextCard(Difficulty.EASY);
                enabled: root.state === 2
            }

            Button {
                id: normalButton

                anchors {
                    left: easyButton.right
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }

                text: qsTr("Normal")
                onClicked: onNextCard(Difficulty.NORMAL);
                enabled: root.state === 2
            }

            Button {
                id: difficultButton

                anchors {
                    left: normalButton.right
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }

                text: qsTr("Difficult")
                onClicked: onNextCard(Difficulty.DIFFICULT);
                enabled: root.state === 2
            }

            Button {
                id: closeButton

                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                text: qsTr("Close")
                onClicked: {
                    controller.closeLearningWindow();
                }
            }
        }
    }

    function onFirstCard() {
        var cardModel = strategy.firstCard();

        if (cardModel.empty) {
            showNothing();
            state = 4;
        } else {
            showFrontSide(cardModel);
        }
    }

    /// @param difficulty difficulty of the card or null, if it is not set(
    ///                   will be used current strategy.currentCard().difficultyRate value)
    function onNextCard(difficulty) {
        if (!(difficulty === null)) {
            strategy.currentCard().difficultyRate = difficulty
        }

        var cardModel = strategy.nextCard();


        if (cardModel.empty) {
            showNothing();
            state = 3;
        } else {
            showFrontSide(cardModel);
        }
    }

    function showNothing() {
        // TODO: implement
        console.log("----------------nothing to show---------------");
    }

    function showFrontSide(cardModel) {
        var viewComponent = Qt.createComponent(cardModel.viewModel.frontViewName);
        var frontView = viewComponent.createObject(null, { "model": cardModel });

        var inputComponent = Qt.createComponent(cardModel.inputModel.inputViewName);
        var inputView = inputComponent.createObject(null, { "model": cardModel });
        inputView.inputCompleted.connect(function() {
            var difficulty = strategy.currentCard().difficultyRate;
            showBackSide(difficulty === Difficulty.UNKNOWN);
        });

        toggleViewport(frontView, inputView);
        state = 1;
    }

    function showBackSide(correctness) {
        var cardModel = strategy.currentCard();
        var viewComponent = Qt.createComponent(cardModel.viewModel.backViewName);
        var backView = viewComponent.createObject(null, { "model": cardModel });
        var statusComponent = Qt.createComponent(cardModel.inputModel.statusViewName);
        var statusView = statusComponent.createObject(null, { "model": cardModel });

        toggleViewport(backView, statusView);
        state = correctness ? 2 : 3;
    }

    function toggleViewport(topComponent, bottomComponent) {
        var elementComponent = Qt.createComponent("ViewportElement.qml");
        var view = elementComponent.createObject(cardViewport, { "children": [ topComponent, bottomComponent ] });

        cardViewport.addItem(view);
        if (cardViewport.count > 1) {
            cardViewport.incrementCurrentIndex();
            cardViewport.removeItem(0);
        }
    }
}
