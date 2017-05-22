#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "Core/translation.h"
#include "server_response.h"
#include "Core/ichange.h"

#include <QObject>

class AuthenticationManager;

class NetworkManager : public QObject {
 Q_OBJECT

 public:
    NetworkManager(QObject *parent = Q_NULLPTR);
    ~NetworkManager();

    quint32 translate(QString source, quint32 timeoutMills = DEFAULT_TIMEOUT_MILLIS);
    /**
     * NOTE: all translations should have the same 'source' field
     */
    quint32 proposeUserTranslations(Translations translations, quint32 timeoutMills = DEFAULT_TIMEOUT_MILLIS);
    quint32 authorize(QString login, QString password, quint32 timeoutMills = DEFAULT_TIMEOUT_MILLIS);
    quint32 registrate(QString login, QString password, quint32 timeoutMills = DEFAULT_TIMEOUT_MILLIS);
    quint32 saveChanges(ChangeList changes, qint32 history_len, quint32 timeoutMillis = DEFAULT_TIMEOUT_MILLIS);
    quint32 loadChanges(qint32 synchronized_prefix_len, quint32 timeoutMillis = DEFAULT_TIMEOUT_MILLIS);

 signals:
    void successTranslate(quint32 id, Translations translations);
    void failTranslate(quint32 id, RPCErrorStatus status);

    void successPropose(quint32 id);
    void failPropose(quint32 id, RPCErrorStatus status);

    void successAuthorize(quint32 id);
    void failAuthorize(quint32 id, RPCErrorStatus status);

    void successRegistrate(quint32 id);
    void failRegistrate(quint32 id, RPCErrorStatus status);

    void successSaveChanges(quint32 id);
    void failSaveChanges(quint32 id, RPCErrorStatus status);

    void successLoadChanges(quint32 id, ChangeList changes);
    void failLoadChanges(quint32 id, RPCErrorStatus status);

    void connectionRestored();
    void connectionBroken(QString description);
 private:
    QAtomicInteger<quint32> currentId_;
    AuthenticationManager *authenticationManager_;
};

#endif // NETWORKMANAGER_H
