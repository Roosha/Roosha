#include <QLabel>

#include "Core/centralcontroller.h"
#include "Helpers/bootstrap.h"
#include "Core/hotkeylistener.h"
#include "Network/networkmanager.h"
#include "GUI/guimanager.h"
#include "GUI/system_tray.h"

#include <QMessageBox>
#include <QApplication>

CentralController::CentralController() {
    Bootstrap boot(this);
    boot.run();

    connect(hkListener, SIGNAL(newWord(QString)), this, SLOT(handleNewWord(QString)));
    connect(systemTray, &SystemTray::newWord, this, &CentralController::handleNewWord);

    connect(networkManager, SIGNAL(newTranslation(TestTranslations)), guiManager, SLOT(showNewTranslationWindow(TestTranslations)));

    connect(systemTray, &SystemTray::closeApplication, this, &CentralController::closeApplication);
}

void CentralController::start() {
    hkListener->start();
}

void CentralController::handleNewWord(QString word) {
    networkManager->requestTranslations(word);
}

void CentralController::closeApplication() {
    QApplication::quit();
}
