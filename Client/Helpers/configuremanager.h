#ifndef CONFIGUREMANAGER_H
#define CONFIGUREMANAGER_H

#include <QString>
#include <QSettings>

class NetworkManager;

class ConfigureManager {
public:

    static ConfigureManager& Instance() {
        static ConfigureManager self_;
        return self_;
    }

    NetworkManager* getNetworkManager();
    void setNetworkManager(NetworkManager* manager);

    QString getLogin();
    QString getPassword();
    QString getToken();

    void setLogin(QString login);
    void setPassword(QString password);
    void setToken(QString token);

protected:
    ConfigureManager();
    ~ConfigureManager();
    ConfigureManager(const ConfigureManager& manager) = delete;
    ConfigureManager& operator=(const ConfigureManager&) = delete;


    NetworkManager* networkManager_;
    QSettings settings_;
};

#endif // CONFIGUREMANAGER_H
