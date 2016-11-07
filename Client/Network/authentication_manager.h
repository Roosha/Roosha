#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

//#include "roosha_service_connector.h"

class ProposeUserTranslationsAsyncCall;
class RegistrateAsyncCall;
class AuthorizeAsyncCall;
class TranslateAsyncCall;
class RooshaServiceConnector;

class TestNetworkManager;

class AuthenticationManager {
public:
    AuthenticationManager(TestNetworkManager *n);

    void translate(TranslateAsyncCall *call);
    void proposeUserTranslation(ProposeUserTranslationsAsyncCall *call);
    void authorize(AuthorizeAsyncCall *call);
    void registrate(RegistrateAsyncCall *call);

private:
    RooshaServiceConnector *connector;
    TestNetworkManager *netManager;
};

#endif // AUTHENTICATIONMANAGER_H
