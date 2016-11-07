#ifndef ROOSHASERVICECONNECTOR_H
#define ROOSHASERVICECONNECTOR_H

#include <QThread>
#include "authentication_manager.h"

class TranslateAsyncCall;
class ProposeUserTranslationsAsyncCall;
class AuthorizeAsyncCall;
class RegistrateAsyncCall;

class AsyncRpcResponseListener;

class RooshaServiceConnector {
public:
    RooshaServiceConnector(AuthenticationManager *m);

    void translate(TranslateAsyncCall *call);
    void proposeUserTranslation(ProposeUserTranslationsAsyncCall *call);
    void authorize(AuthorizeAsyncCall *call);
    void registrate(RegistrateAsyncCall *call);

    void receiveTranslateResponse(TranslateAsyncCall *call);
    void receiveProposeUserTranslationResponse(ProposeUserTranslationsAsyncCall *call);
    void receiveAuthorizeResponse(AuthorizeAsyncCall *call);
    void receiveRegistrateResponse(RegistrateAsyncCall *call);
private:
    AsyncRpcResponseListener *listener;
    AuthenticationManager *authManager;
};

class AsyncRpcResponseListener : public QThread {
    Q_OBJECT
public:
    AsyncRpcResponseListener(RooshaServiceConnector *r);

    void run() Q_DECL_OVERRIDE;
private:
    RooshaServiceConnector *connector;
};

#endif // ROOSHASERVICECONNECTOR_H
