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

    connect(translationController, SIGNAL(createNewCard(QSharedPointer<Translation>)),
            cardCreationController, SLOT(showNewEditWindow(QSharedPointer<Translation>)));
    connect(cardListController, SIGNAL(createNewCard(QSharedPointer<Translation>)),
         cardCreationController, SLOT(showNewEditWindow(QSharedPointer<Translation>)));
    connect(cardCreationController, SIGNAL(showCards()), this, SLOT(showMainWin()));
}

void GUIManager::showNewTranslationWindow(Translations trans) {
    translationController->addData(trans);
}


void GUIManager::showMainWin() {
    cardListController->showCardListWindow();
}
