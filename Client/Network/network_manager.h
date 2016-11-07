#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "Core/translation.h"
//#include "Network/authentication_manager.h"
#include <QObject>

class AuthenticationManager;

enum RPCErrorStatus {

};

class NetworkManager : public QObject { // TODO: rename
    Q_OBJECT

public:
    NetworkManager(QObject * parent = Q_NULLPTR);
    ~NetworkManager();

    quint32 translate(QString source, quint32 timeoutMills);
    quint32 proposeUserTranslations(TestTranslations translations, quint32 timeoutMills);
    quint32 authorize(QString login, QString password, quint32 timeoutMills);
    quint32 registrate(QString login, QString password, quint32 timeoutMills);

signals:
    void newTranslation( TestTranslations trans);

    void successTranslate(quint32 id, TestTranslations translations);
    void failTranslate(quint32 id, RPCErrorStatus status);

    void successPropose(quint32 id);
    void failPropose(quint32 id, RPCErrorStatus status);

    void successAuthorize(quint32 id);
    void failAuthorize(quint32 id, RPCErrorStatus status);

    void successRegistrate(quint32 id);
    void failRegistrate(quint32 id, RPCErrorStatus status);

private:
    AuthenticationManager *authenticationManager;
};

#endif // NETWORKMANAGER_H
