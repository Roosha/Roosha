#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <QObject>
#include <Core/translation.h>
#include "GUI/translationcontroller.h"
#include "GUI/cardcreationcontroller.h"
#include "GUI/cardlistcontroller.h"

class GUIManager : public QObject {
    Q_OBJECT
public:
    GUIManager();

private:
    TranslationController* translationController;
    CardCreationController* cardCreationController;
    CardListController* cardListController;

public slots:
    void showNewTranslationWindow(Translations trans);
    void showMainWin();
};

#endif // GUIMANAGER_H
