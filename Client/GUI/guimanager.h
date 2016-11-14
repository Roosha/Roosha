#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <QObject>
#include <Core/translation.h>
#include "GUI/translationcontroller.h"
#include "GUI/cardcreationcontroller.h"
#include "GUI/cardlistcontroller.h"
#include "GUI/authentication_controller.h"

class GUIManager : public QObject {
    Q_OBJECT
public:
    GUIManager();

private:
    TranslationController* translationController;
    CardCreationController* cardCreationController;
    CardListController* cardListController;
    AuthenticationController* authenticationController;

public slots:
    void showNewTranslationWindow(quint32 id, Translations trans);
    void showMainWin();
    void showAuthenticationWindow();

    void authenticationSuccess(quint32 id);
    void authenticationFail(quint32 id);
};

#endif // GUIMANAGER_H
