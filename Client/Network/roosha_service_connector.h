#ifndef ROOSHASERVICECONNECTOR_H
#define ROOSHASERVICECONNECTOR_H

#include <QThread>

#include <grpc++/grpc++.h>

#include <memory>

#include "authentication_manager.h"
#include "Proto/roosha_service.grpc.pb.h"
#include "server_response.h"

class TranslateAsyncCall;
class ProposeUserTranslationsAsyncCall;
class AuthorizeAsyncCall;
class RegistrateAsyncCall;
class SaveChangesAsyncCall;
class LoadChangesAsyncCall;

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
    void saveChanges(SaveChangesAsyncCall *call);
    void loadChanges(LoadChangesAsyncCall *call);

    void receiveResponse(RpcAsyncCall *call);
 signals:
    void connectionBroken(QString description);
    void connectionRestored();

 private:
    friend class AsyncRpcResponseListener;

    friend class KnockAsyncCall;
    void knock();
    void receivePingResponse(KnockAsyncCall *call);

    void processStatus(grpc::Status status, bool forPingCall = false);

    /// treat as bool
    QAtomicInt isConnectedToServer_;
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
