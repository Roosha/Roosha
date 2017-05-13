#include "GUI/guimanager.h"

GUIManager::GUIManager(QObject *parent) : QObject(parent) {

    qmlRegisterType<Translation>("Translan", 1, 0, "Translation");
    qmlRegisterType<AuthenticationController>("roosha.controllers", 1, 0, "AuthController");
    qmlRegisterType<Card>("roosha.data", 1, 0, "Card"); // TODO: register DBCard in more suitable place.
    LearningWindowController::registerQmlTypes();

    translationController = TranslationController::getInstance();
    cardEditionController = CardEditionController::getInstance();
    cardListController = CardListController::getInstance();
    authenticationController = AuthenticationController::getInstance();
    learningController = LearningWindowController::getInstance();


    qmlRegisterSingletonType<TranslationController>(
            "Roosha.controllers.translation",
            1, 0, "TranslationController",
            register_translation_controller
    );

    qmlRegisterSingletonType<CardEditionController>(
            "Roosha.controllers.cardEdition",
            1, 0, "CardEditionController",
            register_card_edition_controller
    );

    qmlRegisterSingletonType<CardListController>(
            "Roosha.controllers.cardList",
            1, 0, "CardListController",
            register_card_list_controller
    );

    qmlRegisterSingletonType<AuthenticationController>(
            "Roosha.controllers.authentication",
            1, 0, "AuthenticationController",
            register_authentication_controller
    );

    qmlRegisterSingletonType<LearningWindowController>(
            "Roosha.controllers.learning",
            1, 0, "LearningWindowController",
            register_learning_window_controller
    );

    connect(translationController, &TranslationController::createNewCard,
            cardEditionController, &CardEditionController::showNewCardEditWindow);
    connect(cardListController, &CardListController::createNewCard,
            cardEditionController, &CardEditionController::showNewCardEditWindow);
    connect(cardEditionController, &CardEditionController::showCards,
            this, &GUIManager::showMainWindow);
    connect(cardListController, &CardListController::editThisCard,
            cardEditionController, &CardEditionController::showCardEditWindow);
    connect(cardListController, &CardListController::learnCards, learningController,
            &LearningWindowController::showLearningWindow);
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
