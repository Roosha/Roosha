#include "GUI/guimanager.h"
#include <QMessageBox>
#include <QQuickWidget>
#include "GUI/translationcontroller.h"

GUIManager::GUIManager() {

    qmlRegisterType<Translation>("Translan", 1, 0, "Translation");

    translationController = new TranslationController(this);
}

void GUIManager::showNewTranslationWindow(Translations trans) {
    translationController->addData(trans);
}
