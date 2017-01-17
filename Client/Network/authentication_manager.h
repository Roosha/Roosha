#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

#include <QQueue>
#include <QMutex>

#include <string>

#include <grpc++/grpc++.h>

class AuthenticatedAsyncCall;
class AuthorizeOrRegistrateAsyncCall;
class RpcAsyncCall;
class ProposeUserTranslationsAsyncCall;
class RegistrateAsyncCall;
class AuthorizeAsyncCall;
class TranslateAsyncCall;
class RooshaServiceConnector;

class NetworkManager;

class AuthenticationManager {
 public:
    AuthenticationManager(NetworkManager *n);
    ~AuthenticationManager();

    static std::string hashPassword(QString password);

    void authorizeOrRegistrate(AuthorizeOrRegistrateAsyncCall *call);
    void sendWithMetadata(AuthenticatedAsyncCall *call);

    void receiveAuthenticatedCall(AuthenticatedAsyncCall *call);
    void receiveAuthorizeOrRegistrateResponse(AuthorizeOrRegistrateAsyncCall *call);
 private:
    enum State {
        AUTHENTICATED,
        AWAIT_AUTHENTICATION,
        AWAIT_CREDENTIALS,
    };

    static const grpc::string TOKEN_METADATA_KEY;
    static const quint32 TECHNICAL_REQUEST_ID;

    void setState(State state);

    /**
     * Try to get credentials from ConfigurationManager.
     * If got, send new 'authorize' request with TEQNICAL_REQUEST_ID and toggle state_ to AWAIT_AUTHENTICATION.
     * Otherwise, toggle to state_ to AWAIT_CREDENTIALS.
     *
     * NOTE: this function should be called if and only if stateMutex_ is already locked by this thread.
     *
     * @return whether 'authorize' requerst sent or not.
     */
    bool tryToUpdateToken();

    void processAuthorizeOrRegitrateResponse(AuthorizeOrRegistrateAsyncCall *call);

    QMutex stateMutex_;
    State state_;
    QQueue<AuthenticatedAsyncCall *> callsQueue_;
    RooshaServiceConnector *connector_;
    NetworkManager *netManager_;
};

#endif // AUTHENTICATIONMANAGER_H
