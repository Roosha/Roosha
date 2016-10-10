#include <QApplication>
#include <QQmlApplicationEngine>
#include "Core/centralcontroller.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CentralController centralController;
    centralController.start();

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return app.exec();
}
