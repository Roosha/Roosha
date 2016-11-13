#include "Network/network_manager.h"
#include "Network/translation_service.h"
#include "Network/Proto/translation_service.pb.h"
#include "Test/Network/translations_test_slot_holder.h"
#include "Core/dbcard.h"
#include "Core/card.h"
#include "Core/ichange.h"
#include "Core/changes.h"
#include <QApplication>
#include "Core/centralcontroller.h"
#include "Test/Core/changes/worldtest.h"

#include <QDebug>

#include <grpc++/grpc++.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

using roosha::translation::Translations;
TranslationsTestSlotHolder* testTranslationServiceConnection();


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    CentralController controller;
    controller.start();
    return app.exec();
}



TranslationsTestSlotHolder* testTranslationServiceConnection() {
    TranslationsTestSlotHolder* receiver = new TranslationsTestSlotHolder;
    std::shared_ptr<NetworkManager> networkManager(new NetworkManager);
    std::shared_ptr<TranslationService> translationService = networkManager->getTranslationService();

    qRegisterMetaType<TestTranslations>();
    QObject::connect(translationService.get(), &TranslationService::translationSucceeded,
                     receiver, &TranslationsTestSlotHolder::translationSucceededSlot, Qt::DirectConnection);
    QObject::connect(translationService.get(), &TranslationService::translationFailed,
                     receiver, &TranslationsTestSlotHolder::translationFailedSlot, Qt::DirectConnection);

    QStringList str;
    str << "hello" << "gobbles" << "exhibit" << "apple" << "jkajhwvnejkw";
    for (auto cur: str) {
        quint32 id = translationService->translate(cur, 5000u);
        std::cout << "Sent request with id: " << id << std::endl;
    }

    return receiver;
}
