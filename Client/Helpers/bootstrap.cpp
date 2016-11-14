#include "Helpers/bootstrap.h"
#include "Core/centralcontroller.h"
#include "Core/hotkeylistener.h"
#include "Helpers/configuremanager.h"
#include "DB/dbmanager.h"
#include "Network/network_manager.h"
#include "GUI/guimanager.h"
#include "GUI/system_tray.h"

Bootstrap::Bootstrap(CentralController *cc) : cc(cc) {
}

void Bootstrap::run() { // TODO: create all stages of initialisation
    ConfigureManager * cm = &ConfigureManager::Instance();
    NetworkManager * nm = new NetworkManager();
    DBManager * dbm = new DBManager();
    GUIManager * guim = new GUIManager();
    HotkeyListener * hkl = new HotkeyListener();
    SystemTray * sysTray = new SystemTray();

    cm->setNetworkManager(nm);

    qRegisterMetaType<Translations>("Translations");

    cc->configureManager = cm;
    cc->dbManager = dbm;
    cc->guiManager = guim;
    cc->networkManager = nm;
    cc->hkListener = hkl;
    cc->systemTray = sysTray;
}
