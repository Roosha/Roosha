#include "Network/network_manager.h"
#include "Network/Proto/roosha_service.pb.h"
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
    NetworkManager networkManager;

    QObject::connect(&networkManager, &NetworkManager::successTranslate,
                     receiver, &TranslationsTestSlotHolder::translationSucceededSlot, Qt::DirectConnection);
    QObject::connect(&networkManager, &NetworkManager::failTranslate,
                     receiver, &TranslationsTestSlotHolder::translationFailedSlot, Qt::DirectConnection);

    QStringList str;
    str << "hello" << "gobbles" << "exhibit" << "apple" << "jkajhwvnejkw";
    for (auto cur: str) {
        quint32 id = networkManager.translate(cur, 5000u);
        std::cout << "Sent request with id: " << id << std::endl;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1000000));
    return receiver;
}
