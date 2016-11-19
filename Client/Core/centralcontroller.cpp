#include <QLabel>

#include "Core/centralcontroller.h"
#include "Helpers/bootstrap.h"
#include "Core/hotkeylistener.h"
#include "Network/network_manager.h"
#include "GUI/guimanager.h"
#include "GUI/system_tray.h"

#include <QMessageBox>
#include <QApplication>

CentralController::CentralController() {
    Bootstrap boot(this);
    boot.run();

    connect(hkListener, &HotkeyListener::newWord, this, &CentralController::handleNewWord);
    connect(networkManager, &NetworkManager::successTranslate,
            guiManager, &GUIManager::showNewTranslationWindow);


    /**
     * Connecting authentication and registration responces
     */

    qRegisterMetaType<RPCErrorStatus>("RPCErrorStatus");

    connect(networkManager, &NetworkManager::successAuthorize,
            guiManager, &GUIManager::authenticationSuccess);
    connect(networkManager, &NetworkManager::failAuthorize,
            guiManager, &GUIManager::authenticationFail, Qt::ConnectionType::DirectConnection);

    connect(networkManager, &NetworkManager::successRegistrate,
            guiManager, &GUIManager::authenticationSuccess);
    connect(networkManager, &NetworkManager::failRegistrate,
            guiManager, &GUIManager::authenticationFail, Qt::ConnectionType::DirectConnection);

    connect(systemTray, &SystemTray::newWord, this, &CentralController::handleNewWord);
    connect(systemTray, &SystemTray::closeApplication, this, &CentralController::closeApplication);
    connect(systemTray, &SystemTray::showMainWidnow, guiManager, &GUIManager::showMainWindow);
}

void CentralController::start() {
    hkListener->start();
//    guiManager->showMainWin();
    guiManager->showAuthenticationWindow();
}

void CentralController::handleNewWord(QString word) {
    networkManager->translate(word, 5000);
}

void CentralController::closeApplication() {
    QApplication::quit();
}
