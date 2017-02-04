import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.1

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
        var nextCard = strategy.nextCard();
        console.log(nextCard);
        console.log(nextCard.viewMode);
        console.log(nextCard.card);
        console.log(nextCard.card.source);

        cardViewport.incrementCurrentIndex();
        cardViewport.removeItem(0);

    }
}
