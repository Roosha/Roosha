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
    void showNewTranslationWindow(quint32 id, Translations trans);
    void showMainWin();
};

#endif // GUIMANAGER_H
