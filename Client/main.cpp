#include "Network/network_manager.h"
#include "Network/roosha_service.h"
#include "Network/Proto/roosha_service.pb.h"
#include "Test/Network/translations_test_slot_holder.h"
#include "Core/dbcard.h"
#include "Core/card.h"
#include "Core/ichange.h"
#include "Core/changes.h"
#include "Core/cardbuilder.h"
#include <QApplication>
#include "Core/centralcontroller.h"

#include <QDebug>

#include <grpc++/grpc++.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

using roosha::translation::Translations;
TranslationsTestSlotHolder* testTranslationServiceConnection();

void printCard(DBCard card);


void testDBCardBuilder();

int main(int argc, char *argv[]) {
    auto ptr = testTranslationServiceConnection();
    delete ptr;
    testDBCardBuilder();
}

void printCard(DBCard card) {
    qInfo() << "src: ";
    qInfo() << card.getSource();
    qInfo() << "ex: ";
    for (auto ex: card.getExamples()) {
        qInfo() << ex;
    }
    qInfo() << "tar: ";
    for (auto tar: card.getTargets()) {
        qInfo() << tar;
    }
}

void testDBCardBuilder() {
    CardBuilder builder;
    builder.setSource("src1");

    builder.insertExample("ex1", 0); //ex1
    builder.insertExample("ex2", 0); //ex2 ex1
    builder.deleteExample(1);        //ex2
    builder.insertExample("ex3", 0); //ex3 ex2
    builder.editExample("ex4", 0);   //ex4 ex2

    builder.insertTarget("tar1", 0); //tar1
    builder.insertTarget("tar0", 0); //tar0 tar1
    builder.insertTarget("tar2", 1); //tar0 tar2 tar1
    DBCard card = builder.build();

    qInfo() << "card after build: ";
    printCard(card);


    card.setSource("src0");
    card.deleteExampleElem(0);        //ex2
    card.insertExampleElem("ex5", 0); //ex5 ex2
    card.insertExampleElem("ex6", 0); //ex6 ex5 ex2
    card.editExampleElem("ex", 2);    //ex6 ex5 ex
    card.insertTargetElem("tar4", 3); //tar0 tar2 tar1 tar4
    card.editTargetElem("tar", 1);    //tar0 tar tar1 tar4
    card.deleteTargetElem(2);         //tar0 tar2 tar4

    qInfo() << "card after changes out builder: ";
    printCard(card);

    card.applyChanges();

    qInfo() << "card after reset and apply changes: ";
    printCard(card);

}

TranslationsTestSlotHolder* testTranslationServiceConnection() {
    TranslationsTestSlotHolder* receiver = new TranslationsTestSlotHolder;
    std::shared_ptr<NetworkManager> networkManager(new NetworkManager);
    std::shared_ptr<RooshaRpcService> translationService = networkManager->getTranslationService();

    qRegisterMetaType<TestTranslations>();
    QObject::connect(translationService.get(), &RooshaRpcService::translationSucceeded,
                     receiver, &TranslationsTestSlotHolder::translationSucceededSlot, Qt::DirectConnection);
    QObject::connect(translationService.get(), &RooshaRpcService::translationFailed,
                     receiver, &TranslationsTestSlotHolder::translationFailedSlot, Qt::DirectConnection);

    QStringList str;
    str << "hello" << "gobbles" << "exhibit" << "apple" << "jkajhwvnejkw";
    for (auto cur: str) {
        quint32 id = translationService->translate(cur, 5000u);
        std::cout << "Sent request with id: " << id << std::endl;
    }

    return receiver;
}
