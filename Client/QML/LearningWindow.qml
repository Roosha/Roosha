import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1
import roosha.learning 1.0
import roosha.data 1.0

Rectangle {
    height: 500
    width: 500
    Component.onCompleted: {
        createNextCard();
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
                id: easyButton

                anchors {
                    left: parent.left
                    verticalCenter: parent.verticalCenter
                }

                text: qsTr("Easy")
                onClicked: {
                    createNextCard();
                    showNextCard();
                }
            }

            Button {
                id: normalButton

                anchors {
                    left: easyButton.right
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }

                text: qsTr("Normal")
                onClicked: {
                    createNextCard();
                    showNextCard();
                }
            }

            Button {
                id: difficultButton

                anchors {
                    left: normalButton.right
                    leftMargin: 10
                    verticalCenter: parent.verticalCenter
                }

                text: qsTr("Difficult")
                onClicked: {
                    createNextCard();
                    showNextCard();
                }
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

    function createNextCard() {
        var component = Qt.createComponent("CardLearningView.qml");
        cardViewport.addItem(
                component.createObject(cardViewport, {text: "Card " + Math.random(10)})
        );
    }

    function showNextCard() {
        var nextCardModel = strategy.firstCard();
        console.log(nextCardModel);
        var card = nextCardModel.card
        console.log('card: ',  card)
        console.log('card source: ',  card.source)

        var view = nextCardModel.viewModel
        console.log('viewModel: ',  view)
        console.log('front view: ',  view.frontViewName)
        console.log('back view: ',  view.backViewName)

        var input = nextCardModel.inputModel
        console.log('inputModel: ', input)
        console.log('input view: ', input.viewName)

        console.log('------------------')

        nextCardModel = strategy.nextCard(Difficulty.FAILED);
        console.log(nextCardModel);
        card = nextCardModel.card
        console.log('card: ',  card)
        console.log('card source: ',  card.source)

        view = nextCardModel.viewModel
        console.log('viewModel: ',  view)
        console.log('front view: ',  view.frontViewName)
        console.log('back view: ',  view.backViewName)

        input = nextCardModel.inputModel
        console.log('inputModel: ', input)
        console.log('input view: ', input.viewName)


        cardViewport.incrementCurrentIndex();
        cardViewport.removeItem(0);

    }
}
