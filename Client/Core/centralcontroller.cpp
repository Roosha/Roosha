#include <QLabel>

#include "Core/centralcontroller.h"
#include "Helpers/bootstrap.h"
#include "Core/hotkeylistener.h"
#include "Network/network_manager.h"
#include "GUI/guimanager.h"
#include <QMessageBox>

CentralController::CentralController() {
    Bootstrap boot(this);
    boot.run();

    connect(hkListener, &HotkeyListener::newWord, this, &CentralController::handleNewWord);
    connect(networkManager, &NetworkManager::successTranslate,
            guiManager, &GUIManager::showNewTranslationWindow);
}

void CentralController::start() {
    hkListener->start();
    guiManager->showMainWin();
}

void CentralController::handleNewWord(QString word) {
    networkManager->translate(word, 5000);
}
