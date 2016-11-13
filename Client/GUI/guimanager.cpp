#include "GUI/guimanager.h"
#include <QMessageBox>
#include <QQuickWidget>
#include "GUI/translationcontroller.h"
#include "GUI/cardcreationcontroller.h"

GUIManager::GUIManager() {

    qmlRegisterType<Translation>("Translan", 1, 0, "Translation");

    translationController = new TranslationController(this);
    cardCreationController = new CardCreationController(this);
    cardListController = new CardListController(this);

    connect(translationController, &TranslationController::createNewCard,
            cardCreationController, &CardCreationController::showNewEditWindow);
    connect(cardListController, &CardListController::createNewCard,
            cardCreationController, &CardCreationController::showNewEditWindow);
    connect(cardCreationController, &CardCreationController::showCards,
            this, &GUIManager::showMainWin);
}

void GUIManager::showNewTranslationWindow(quint32 id, Translations trans) {
    translationController->addData(trans);
}


void GUIManager::showMainWin() {
    cardListController->showCardListWindow();
}
