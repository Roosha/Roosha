

//#define DAEMON // uncomment, if you want to run application in daemon mode


//#if defined DAEMON
//    #include "Core/applicationdaemon.h"
//#else
//    #include <QApplication>
//    #include "Core/centralcontroller.h"
//#endif
#include "Core/dbcard.h"
#include "Core/card.h"
#include "Core/changes.h"
#include "Core/ichange.h"
#include "Core/cardbuilder.h"

#include <QDebug>

void test() {
    CardBuilder builder;
    builder.setSource("src1");
    DBCard card = builder.build();
    qInfo() << card.getSource();
    card.setSource("src2");
    qInfo() << card.getSource();
    card.applyChanges();
    qInfo() << card.getSource();
}

int main(int argc, char *argv[])
{
//#if defined DAEMON
//    ApplicationDaemon daemon(argc, argv);  // not confortable to debugging daemon
//    return daemon.exec();
//#else
//    QApplication app(argc, argv);

//    app.setQuitOnLastWindowClosed(false);

//    CentralController centralController;
//    centralController.start();

//    return app.exec();
//#endif
    test();
}


// QML:
//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
//#include <QQmlApplicationEngine>
