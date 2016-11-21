#include "Helpers/bootstrap.h"
#include "Core/centralcontroller.h"
#include "Core/hotkeylistener.h"
#include "Helpers/configuremanager.h"
#include "Network/network_manager.h"
#include "GUI/guimanager.h"
#include "GUI/system_tray.h"

Bootstrap::Bootstrap(CentralController *cc) : cc_(cc) {
}

void Bootstrap::run() { // TODO: create all stages of initialisation
    ConfigureManager * cm = &ConfigureManager::Instance();
    /**
    * NOTE: {@link CardListController} uses {@link ConfigureManager::getNetworkManager} in constructor,
    * so {@link GUIManager} should be created after network manager is set in confifure manager.
    */
    NetworkManager * nm = new NetworkManager(cc_);
    cm->setNetworkManager(nm);


    GUIManager * guim = new GUIManager(cc_);
    HotkeyListener * hkl = new HotkeyListener(cc_);

    QSharedPointer<SystemTray> sysTray(new SystemTray(), &QObject::deleteLater);

    qRegisterMetaType<Translations>("Translations");

    cc_->configureManager = cm;
    cc_->guiManager = guim;
    cc_->networkManager = nm;
    cc_->hkListener = hkl;
    cc_->systemTray = sysTray;
}
