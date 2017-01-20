#include "Network/network_manager.h"
#include <QApplication>
#include "Core/centralcontroller.h"
#include "GUI/system_tray.h"

SystemTray* SystemTray::tray_ = Q_NULLPTR;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    CentralController controller;
    controller.start();
    return app.exec();
}
