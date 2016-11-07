#ifndef ROOSHASERVICECONNECTOR_H
#define ROOSHASERVICECONNECTOR_H

#include <QThread>

#include <grpc++/grpc++.h>

#include "authentication_manager.h"
#include "Proto/roosha_service.grpc.pb.h"

class TranslateAsyncCall;
class ProposeUserTranslationsAsyncCall;
class AuthorizeAsyncCall;
class RegistrateAsyncCall;

class AsyncRpcResponseListener;

class RooshaServiceConnector : public QObject {
    Q_OBJECT
public:
    RooshaServiceConnector(AuthenticationManager *m);
    ~RooshaServiceConnector();

    void translate(TranslateAsyncCall *call);
    void proposeUserTranslation(ProposeUserTranslationsAsyncCall *call);
    void authorize(AuthorizeAsyncCall *call);
    void registrate(RegistrateAsyncCall *call);

    void receiveTranslateResponse(TranslateAsyncCall *call);
    void receiveProposeUserTranslationResponse(ProposeUserTranslationsAsyncCall *call);
    void receiveAuthorizeResponse(AuthorizeAsyncCall *call);
    void receiveRegistrateResponse(RegistrateAsyncCall *call);
private:
    friend class AsyncRpcResponseListener;

    grpc::CompletionQueue completionQueue_;
    std::unique_ptr<roosha::RooshaService::Stub> stub_;
    AsyncRpcResponseListener *responseListener_;
    AuthenticationManager *authManager_;
};

class AsyncRpcResponseListener : public QThread {
    Q_OBJECT
public:
    AsyncRpcResponseListener(RooshaServiceConnector *r);

    void run() Q_DECL_OVERRIDE;
private:
    RooshaServiceConnector *connector_;
};

#endif // ROOSHASERVICECONNECTOR_H
