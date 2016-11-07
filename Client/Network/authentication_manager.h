#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

//#include "roosha_service_connector.h"

class ProposeUserTranslationsAsyncCall;
class RegistrateAsyncCall;
class AuthorizeAsyncCall;
class TranslateAsyncCall;
class RooshaServiceConnector;

class NetworkManager;

class AuthenticationManager {
public:
    AuthenticationManager(NetworkManager *n);

    void translate(TranslateAsyncCall *call);
    void proposeUserTranslation(ProposeUserTranslationsAsyncCall *call);
    void authorize(AuthorizeAsyncCall *call);
    void registrate(RegistrateAsyncCall *call);

    void receiveTranslateResponse(TranslateAsyncCall *call);
    void receiveProposeUserTranslationResponse(ProposeUserTranslationsAsyncCall *call);
    void receiveAuthorizeResponse(AuthorizeAsyncCall *call);
    void receiveRegistrateResponse(RegistrateAsyncCall *call);
private:
    RooshaServiceConnector *connector_;
    NetworkManager *netManager_;
};

#endif // AUTHENTICATIONMANAGER_H
