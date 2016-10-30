#include "builderandchangestest.h"
#include "Core/dbcard.h"
#include "Core/cardbuilder.h"
#include "Core/changes.h"
#include <QtDebug>

BuilderAndChangesTest::BuilderAndChangesTest() { }
void BuilderAndChangesTest::printCard(DBCard card) {
    qInfo() << "id: " << card.getId();
    qInfo() << "src: " << card.getSource();
    qInfo() << "ex: ";
    for (auto ex: card.getExamples()) {
        qInfo() << ex;
    }
    qInfo() << "tar: ";
    for (auto tar: card.getTargets()) {
        qInfo() << tar;
    }
}

void BuilderAndChangesTest::run() {
    CardBuilder builder;
    builder.setSource("src1");

    builder.insert(EXAMPLE,"ex1", 0).insert(EXAMPLE,"ex2", 0).del(EXAMPLE, 1)
            .insert(EXAMPLE,"ex3", 0).edit(EXAMPLE, "ex4", 0) //ex4 ex2
            .insert(TARGET, "tar1", 0).insert(TARGET, "tar0", 0).insert(TARGET, "tar2", 1); //tar0 tar2 tar1
    DBCard card = builder.build();

    qInfo() << "card after build: ";
    printCard(card);


//    card.setSource("src0");
//    card.deleteElem(EXAMPLE, 0);        //ex2
//    card.insertElem(EXAMPLE, "ex5", 0); //ex5 ex2
//    card.insertElem(EXAMPLE, "ex6", 0); //ex6 ex5 ex2
//    card.editElem(EXAMPLE, "ex", 2);    //ex6 ex5 ex
//    card.insertElem(TARGET, "tar4", 3); //tar0 tar2 tar1 tar4
//    card.editElem(TARGET, "tar", 1);    //tar0 tar tar1 tar4
//    card.deleteElem(TARGET, 2);         //tar0 tar tar4

//    qInfo() << "card after changes out builder: ";
//    printCard(card);

//    card.applyChanges();

//    qInfo() << "card after reset and apply changes: ";
//    printCard(card);

}
