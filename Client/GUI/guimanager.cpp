#include "GUI/guimanager.h"
#include <QMessageBox>
#include <QQuickWidget>
#include "GUI/translationcontroller.h"
#include "GUI/cardcreationcontroller.h"
#include "GUI/authentication_controller.h"

GUIManager::GUIManager() {

    qmlRegisterType<Translation>("Translan", 1, 0, "Translation");
    qmlRegisterType<AuthenticationController>("roosha.controllers", 1, 0, "AuthController");

    translationController = new TranslationController(this);
    cardCreationController = new CardCreationController(this);
    cardListController = new CardListController(this);
    authenticationController = new AuthenticationController(this);

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

void GUIManager::showAuthenticationWindow() {
    authenticationController->showLoginWindow();
}

void GUIManager::authenticationSuccess(quint32 id) {
    authenticationController->authenticationSuccess(id);
}

void GUIManager::authenticationFail(quint32 id, RPCErrorStatus status) {
    authenticationController->authenticationFail(id);
}
