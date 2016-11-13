#include "worldtest.h"
#include "Core/world.h"
#include <QUuid>
#include <QDebug>

WorldTest::WorldTest() { }

void WorldTest::printCard(QSharedPointer<DBCard> card) {
    qInfo() << "id: " << card->getId();
    qInfo() << "src: " << card->getSource();
    qInfo() << "ex: ";
    for (auto ex: card->getExamples()) {
        qInfo() << ex;
    }
    qInfo() << "tar: ";
    for (auto tar: card->getTargets()) {
        qInfo() << tar;
    }
}

void WorldTest::printCards(World * world) {
    QMap<QUuid, QSharedPointer<DBCard>>::const_iterator i = world->getCards().constBegin();
    while (i != world->getCards().constEnd()) {
        qInfo() << i.key() << ": ";
        printCard(i.value());
        ++i;
    }
}

void WorldTest::run() {
    World& world = World::Instance();
    QSharedPointer<DBCard> card = world.createCard();
    card->insertElem(EXAMPLE,"ex1", 0);
    card->setSource("src1");
    card->insertElem(EXAMPLE,"ex2", 0);
    card->deleteElem(EXAMPLE, 1);
    card->insertElem(EXAMPLE,"ex3", 0);
    card->editElem(EXAMPLE, "ex4", 0);
    card->insertElem(TARGET, "tar", 0);
    card = world.createCard();
    QUuid id_to_del = card->getId();
    card->setSource("src");
    card->insertElem(EXAMPLE,"ex", 0);
    card->deleteElem(EXAMPLE, 0);
    card->insertElem(EXAMPLE,"ex3", 0);
    card->editElem(EXAMPLE, "ex4", 0);
    card->insertElem(TARGET, "t", 0);
    card = world.createCard();
    card->setSource("src");
    card->insertElem(EXAMPLE,"ex", 0);
    card->deleteElem(EXAMPLE, 0);
    card->insertElem(EXAMPLE,"ex", 0);
    card->editElem(EXAMPLE, "e", 0);
    card->insertElem(TARGET, "tr", 0);
    printCards(&world);
    qInfo() << "after remove \n";
    world.deleteCard(id_to_del);
    printCards(&world);
    world.applyChanges();
    qInfo() << "after reapplying in world\n";
    printCards(&world);
}

