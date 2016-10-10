#include "Helpers/bootstrap.h"
#include "Core/centralcontroller.h"
#include "Core/hotkeylistener.h"
#include "Helpers/configuremanager.h"
#include "DB/dbmanager.h"
#include "Network/networkmanager.h"
#include "GUI/guimanager.h"

Bootstrap::Bootstrap(CentralController *cc) : cc(cc) {
}

void Bootstrap::run() { // TODO: create all stages of initialisation
    ConfigureManager * cm = new ConfigureManager();
    NetworkManager * nm = new NetworkManager();
    DBManager * dbm = new DBManager();
    GUIManager * guim = new GUIManager();
    HotkeyListener * hkl = new HotkeyListener();

    cc->configureManager = cm;
    cc->dbManager = dbm;
    cc->guiManager = guim;
    cc->networkManager = nm;
    cc->hkListener = hkl;
}
