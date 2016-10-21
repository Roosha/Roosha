#include <QApplication>
#include "Core/centralcontroller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setQuitOnLastWindowClosed(false);

    CentralController centralController;
    centralController.start();

    return app.exec();
}
