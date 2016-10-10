#include "Core/centralcontroller.h"
#include "Helpers/bootstrap.h"
#include "Core/hotkeylistener.h"
#include <QMessageBox>

CentralController::CentralController() {
    Bootstrap boot(this);
    boot.run();

    connect(hkListener, SIGNAL(newWord(QString)), this, SLOT(handleNewWord(QString)));
}

void CentralController::start() {
    hkListener->startListen();
}

void CentralController::handleNewWord(QString word) {
    QMessageBox m;
    m.setText(word);
    m.exec();
}
