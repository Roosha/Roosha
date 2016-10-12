#include <QApplication>
#include <QQmlApplicationEngine>
#include "Core/centralcontroller.h"

#include "Network/networkmanager.h"
#include "Network/translation_service.h"
#include "Network/Proto/translation_service.pb.h"

#include <iostream>

int main(int argc, char *argv[])
{
    testNetwork();

    //    QApplication app(argc, argv);

//    CentralController centralController;
//    centralController.start();

//    QQmlApplicationEngine engine;
//    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    return 0;
}

void testNetwork() {
    NetworkManager networkManager = NetworkManager::getInstance();
    std::shared_ptr<TranslationService> translationService = networkManager.getTranslationService();
    roosha::translation::Translations translations = translationService->translate("exhibit");
    std::cout << translations.SerializeAsString();
}