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

    connect(hkListener, SIGNAL(newWord(QString)), this, SLOT(handleNewWord(QString)));
    connect(networkManager, SIGNAL(newTranslation(TestTranslations)), guiManager, SLOT(showNewTranslationWindow(TestTranslations)));
}

void CentralController::start() {
    hkListener->start();
}

void CentralController::handleNewWord(QString word) {
    networkManager->translate(word);
}
