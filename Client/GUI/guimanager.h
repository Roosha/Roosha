#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <QObject>
#include <Core/translation.h>
#include "GUI/translationcontroller.h"

class GUIManager : public QObject {
    Q_OBJECT
public:
    GUIManager();

private:
    TranslationController* translationController;

public slots:
    void showNewTranslationWindow(TestTranslations trans);
};

#endif // GUIMANAGER_H
