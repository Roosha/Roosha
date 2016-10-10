

//#define DAEMON // uncomment, if you want to run application in daemon mode


#if defined DAEMON
    #include "Core/applicationdaemon.h"
#else
    #include <QApplication>
    #include "Core/centralcontroller.h"
#endif

int main(int argc, char *argv[])
{
#if defined DAEMON
    ApplicationDaemon daemon(argc, argv);  // not confortable to debugging daemon
    return daemon.exec();
#else
    QApplication app(argc, argv);

    app.setQuitOnLastWindowClosed(false);

    CentralController centralController;
    centralController.start();

    return app.exec();
#endif
}


// QML:
//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//#include <QQmlApplicationEngine>
