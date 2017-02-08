import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import roosha.learning 1.0
import roosha.data 1.0

Rectangle {
    id: root
    height: 500
    width: 500
    Component.onCompleted: onFirstCard();

    // 0 -- initial, 1 -- front side, 2 -- back side, 3 -- nothing to change
    property int state: 0

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
                id: easyButton

                anchors {
                    left: parent.left
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
            state = 3;
        } else {
            showFrontSide(cardModel);
        }
    }

    function onNextCard(difficulty) {
        var cardModel = strategy.nextCard(difficulty);


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
        console.log("Trying to load object", cardModel.viewModel.frontViewName);
        console.log("cardModel.viewModel: ", cardModel.viewModel);

        var viewComponent = Qt.createComponent(cardModel.viewModel.frontViewName);
        var frontView = viewComponent.createObject(null, {
                "card": cardModel.card,
                "model": cardModel.viewModel
        });
        console.log("frontView.height", frontView.height);
        console.log("Trying to load object", cardModel.inputModel.inputViewName);

        var inputComponent = Qt.createComponent(cardModel.inputModel.inputViewName);
        var inputView = inputComponent.createObject(null, {
                "card": cardModel.card,
                "model": cardModel.inputModel,
        });
        inputView.inputCompleted.connect(function() {
            console.log("handling `inputCompleted` signal.");
            showBackSide();
        });

        toggleViewport(frontView, inputView);
        state = 1;
    }

    function showBackSide() {
        console.log("showBackSide called");

        var cardModel = strategy.currentCard();
        var viewComponent = Qt.createComponent(cardModel.viewModel.backViewName);
        var backView = viewComponent.createObject(null, {
                "card": cardModel.card,
                "model": cardModel.viewModel
        });
        var statusComponent = Qt.createComponent(cardModel.inputModel.statusViewName);
        var statusView = statusComponent.createObject(null, {
                "card": cardModel.card,
                "model": cardModel.inputModel
        });

        toggleViewport(backView, statusView);
        state = 2;
        console.log("state = ", state);
        console.log("state === 2", state === 2);
    }

    function toggleViewport(topComponent, bottomComponent) {
        var elementComponent = Qt.createComponent("ViewportElement.qml");
        var view = elementComponent.createObject(cardViewport, {
                "children": [
                    topComponent,
                    bottomComponent
                ]
        });

        cardViewport.addItem(view);
        console.log("cardViewport.currentIndex: ", cardViewport.currentIndex);
        console.log("cardViewport.count : ", cardViewport.count);
        if (cardViewport.count > 1) {
            cardViewport.incrementCurrentIndex();
            cardViewport.removeItem(0);
        }
    }
}
