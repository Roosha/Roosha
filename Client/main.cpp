#include "Network/network_manager.h"
#include <QApplication>
#include "Core/centralcontroller.h"

int main(int argc, char *argv[]) {
    srand((unsigned int) time(NULL));

    QApplication app(argc, argv);


    app.setQuitOnLastWindowClosed(false);
    CentralController controller;
    controller.start();
    return app.exec();
}