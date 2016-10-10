//#include <QApplication>
//#include <QQmlApplicationEngine>
//#include "Core/centralcontroller.h"
#include "Core/applicationdaemon.h"

int main(int argc, char *argv[])
{
    ApplicationDaemon daemon(argc, argv);
    return daemon.exec();
//    QApplication app(argc, argv);

//    CentralController centralController;
//    centralController.start();

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//    return app.exec();
}
