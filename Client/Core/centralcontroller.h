#ifndef CENTRALCONTROLLER_H
#define CENTRALCONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include <Helpers/StateHolder.h>
#include "Core/translation.h"

class Bootstrap;
class ConfigureManager;
class DBManager;
class NetworkManager;
class GUIManager;
class HotkeyListener;
class SystemTray;

class CentralController : public QObject {
 Q_OBJECT

    friend class ::Bootstrap;
 public:
    CentralController();
    void start();

 private:
    ConfigureManager *configureManager;
    NetworkManager *networkManager;
    GUIManager *guiManager;
    HotkeyListener *hkListener;
    StateHolder *stateHolder;
    QSharedPointer<SystemTray> systemTray;


 public slots:
    void handleNewWord(QString word);
    void closeApplication();
};

#endif // CENTRALCONTROLLER_H
