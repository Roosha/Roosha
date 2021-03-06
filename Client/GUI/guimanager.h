#ifndef GUIMANAGER_H
#define GUIMANAGER_H

#include <QObject>
#include <Core/translation.h>
#include <GUI/Learning/LearningWindowController.h>
#include "GUI/translationcontroller.h"
#include "GUI/cardeditioncontroller.h"
#include "GUI/cardlistcontroller.h"
#include "GUI/authentication_controller.h"

#include "Network/server_response.h"

class GUIManager : public QObject {
 Q_OBJECT
 public:
    GUIManager(QObject *parent = Q_NULLPTR);

 private:
    TranslationController *translationController;
    CardEditionController *cardEditionController;
    CardListController *cardListController;
    AuthenticationController *authenticationController;
    LearningWindowController *learningController;

 public slots:
    void showNewTranslationWindow(quint32 id, Translations trans);
    void showMainWindow();
    void showAuthenticationWindow();

    void authenticationSuccess(quint32 id);
    void authenticationFail(quint32 id, RPCErrorStatus status);
};

#endif // GUIMANAGER_H
