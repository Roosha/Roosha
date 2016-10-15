#include <QLabel>

#include "Core/centralcontroller.h"
#include "Helpers/bootstrap.h"
#include "Core/hotkeylistener.h"

CentralController::CentralController() {
    Bootstrap boot(this);
    boot.run();

    connect(hkListener, SIGNAL(newWord(QString)), this, SLOT(handleNewWord(QString)));
}

void CentralController::start() {
    hkListener->start();
}

void CentralController::handleNewWord(QString word) {
    QLabel * label = new QLabel(word); //TODO: make a request trought NetManager and show mini window with transaltion #Roosha-21
    label->show();
}
