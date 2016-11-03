#ifndef AUTHENTICATIONMANAGER_H
#define AUTHENTICATIONMANAGER_H

class GeneralPurposeAsyncCall;
class RegistrateAsyncCall;
class AuthorizeAsyncCall;

class AuthenticationManager {
public:
    AuthenticationManager();

    void handle(GeneralPurposeAsyncCall *call);
    void handle(RegistrateAsyncCall *call);
    void handle(AuthorizeAsyncCall *call);
};

#endif // AUTHENTICATIONMANAGER_H
