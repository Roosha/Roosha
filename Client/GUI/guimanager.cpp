#include "GUI/guimanager.h"
#include <QMessageBox>
#include <QQuickWidget>
#include "GUI/translationcontroller.h"
#include "GUI/cardeditioncontroller.h"
#include "GUI/authentication_controller.h"

GUIManager::GUIManager(QObject *parent): QObject(parent) {

    qmlRegisterType<Translation>("Translan", 1, 0, "Translation");
    qmlRegisterType<AuthenticationController>("roosha.controllers", 1, 0, "AuthController");

    translationController = new TranslationController(this);
    cardEditionController = new CardEditionController(this);
    cardListController = new CardListController(this);
    authenticationController = new AuthenticationController(this);

    connect(translationController, &TranslationController::createNewCard,
            cardEditionController, &CardEditionController::showNewCardEditWindow);
    connect(cardListController, &CardListController::createNewCard,
            cardEditionController, &CardEditionController::showNewCardEditWindow);
    connect(cardEditionController, &CardEditionController::showCards,
            this, &GUIManager::showMainWindow);
    connect(cardListController, &CardListController::editThisCard,
            cardEditionController, &CardEditionController::showCardEditWindow);
}

void GUIManager::showNewTranslationWindow(quint32 id, Translations trans) {
    translationController->addData(trans);
}


void GUIManager::showMainWindow() {
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
