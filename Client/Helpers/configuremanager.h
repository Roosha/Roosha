#ifndef CONFIGUREMANAGER_H
#define CONFIGUREMANAGER_H

#define MAX_CM 3
#include <QString>
#include <QSettings>

class Synchronizer;
class NetworkManager;

class ConfigureManager {
 public:

    static ConfigureManager &Instance() {
        static ConfigureManager self_[MAX_CM];
        return self_[ConfigureManager::version];
    }

    NetworkManager *getNetworkManager();
    Synchronizer *getSynchronizer();
    void setNetworkManager(NetworkManager *manager);
    void setSynchronizer(Synchronizer *synchronizer);

    QString getLogin();
    QString getPasswordHash();
    QString getToken();
    qint8 getMachineId();
    qint32 getSyncLength();

    void setLogin(QString login);
    void setPasswordHash(QString password);
    void setToken(QString token);
    void setMachineId(qint8 machineId);
    void setSyncLength(qint32 len);

    static qint8 version;
 protected:
    ConfigureManager();
    ~ConfigureManager();
    ConfigureManager(const ConfigureManager &manager) = delete;
    ConfigureManager &operator=(const ConfigureManager &) = delete;

    Synchronizer *synchronizer_;
    NetworkManager *networkManager_;
    QSettings settings_;
    qint8 testMID;
};

#endif // CONFIGUREMANAGER_H
