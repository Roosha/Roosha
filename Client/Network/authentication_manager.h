#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

#include <QQueue>
#include <QMutex>

#include <string>

#include <grpc++/grpc++.h>

#include "preferences.h"
#include "error_status.h"

class AuthenticatedAsyncCall;
class AuthorizeOrRegistrateAsyncCall;
class RpcAsyncCall;
class ProposeUserTranslationsAsyncCall;
class RegistrateAsyncCall;
class AuthorizeAsyncCall;
class TranslateAsyncCall;
class RooshaServiceConnector;

class NetworkManager;

class AuthenticationManager : public QObject {
 Q_OBJECT
 public:
    AuthenticationManager(NetworkManager *n);
    ~AuthenticationManager();

    static std::string hashPassword(QString password);

    void authorizeOrRegistrate(AuthorizeOrRegistrateAsyncCall *call);
    void sendWithMetadata(AuthenticatedAsyncCall *call);

    void receiveAuthenticatedCall(AuthenticatedAsyncCall *call);
    void receiveAuthorizeOrRegistrateResponse(AuthorizeOrRegistrateAsyncCall *call);
 private:
    //@formatter:off
    enum State {
        /// current token is correct
        AUTHENTICATED,
        /// there is a pending authentication RPC.
        AWAIT_AUTHENTICATION,
        /// both token and credentials in configuration are invalid
        AWAIT_CREDENTIALS,
        /// there is no valid token and validity of credentials in configuration is unknown due to disconnection
        /// from the server. 'authenticate' RPC should be called as soon as connection is restored.
        ///
        /// <b>NOTE:</b> this state should be set only after RooshaServiceConnector::connectionBroken or
        /// RooshaServiceConnector::connectionRestored signal is emitted.
        AWAIT_CONNECTION,
    };
    //@formatter:on

    void setState(State state);

    /**
     * Try to get credentials from ConfigurationManager.
     * If got, send new 'authorize' request with TECHNICAL_REQUEST_ID and toggle state_ to AWAIT_AUTHENTICATION.
     * Otherwise, toggle to state_ to AWAIT_CREDENTIALS.
     *
     * NOTE: this function should be called if and only if stateMutex_ is already locked by this thread.
     *
     * @return whether 'authorize' requerst sent or not.
     */
    bool tryToUpdateToken();

    void processAuthorizeOrRegistrateResponse(AuthorizeOrRegistrateAsyncCall *call);

    void onConnectionBroken();
    void onConnectionRestored();

    void failAllQueuedCalls(RPCErrorStatus status);
    void retryAllQueuedCalls();

    QMutex stateMutex_;
    State state_;
    QQueue<AuthenticatedAsyncCall *> callsQueue_;
    RooshaServiceConnector *connector_;
    NetworkManager *netManager_;
};

#endif // AUTHENTICATIONMANAGER_H
