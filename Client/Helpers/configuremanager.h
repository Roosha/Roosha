#ifndef CONFIGUREMANAGER_H
#define CONFIGUREMANAGER_H

#include <QString>
#include <QSettings>

class Synchronizer;
class NetworkManager;

class ConfigureManager {
 public:

    static ConfigureManager &Instance() {
        static ConfigureManager self_;
        return self_;
    }

    NetworkManager *getNetworkManager();
    Synchronizer *getSynchronizer();
    void setNetworkManager(NetworkManager *manager);
    void setSynchronizer(Synchronizer *synchronizer);

    QString getLogin();
    QString getPasswordHash();
    QString getToken();

    void setLogin(QString login);
    void setPasswordHash(QString password);
    void setToken(QString token);

 protected:
    ConfigureManager();
    ~ConfigureManager();
    ConfigureManager(const ConfigureManager &manager) = delete;
    ConfigureManager &operator=(const ConfigureManager &) = delete;

    Synchronizer *synchronizer_;
    NetworkManager *networkManager_;
    QSettings settings_;
};

#endif // CONFIGUREMANAGER_H
