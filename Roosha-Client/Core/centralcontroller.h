#ifndef CENTRALCONTROLLER_H
#define CENTRALCONTROLLER_H

#include <QObject>
#include <QSharedPointer>
#include "Core/translation.h"

class Bootstrap;
class ConfigureManager;
class DBManager;
class NetworkManager;
class GUIManager;
class HotkeyListener;

class CentralController : public QObject {
    Q_OBJECT

friend class ::Bootstrap;
 public:
    CentralController();

    void start();

 private:
    ConfigureManager * configureManager;
    NetworkManager * networkManager;
    DBManager * dbManager;
    GUIManager * guiManager;
    HotkeyListener * hkListener;

public slots:
    void handleNewWord(QString word);
};

#endif // CENTRALCONTROLLER_H
