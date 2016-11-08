#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

#include <grpc++/grpc++.h>
#include <QQueue>
#include <QMutex>

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
    static const grpc::string  TOKEN_METADATA_KEY;
    static const quint32 TECHNICAL_REQUEST_ID;

    /**
     * This function should be called only when stateMutex_ is locked.
     * @note authorizationRequest is sent with id == TECHNICAL_REQUEST_ID,
     * so result of this request should not be emited outside NetworkManager
     * @return {@code true} if authorization request sent(and current state is AWAIT_AUTHENTICATION) and {@code false}
     * if no authorization request sent and current status is AWAIT_CREDENTIALS.
     */
    bool tryToUpdateToken();
    void processAuthorizeOrRegitrateResponse(AuthorizeOrRegistrateAsyncCall *call);

    QMutex stateMutex_;
    State state_;
    grpc::string token_;
    QQueue<AuthenticatedAsyncCall*> callsQueue_;
    RooshaServiceConnector *connector_;
    NetworkManager *netManager_;
};

#endif // AUTHENTICATIONMANAGER_H
