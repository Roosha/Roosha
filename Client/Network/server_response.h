#ifndef ASYNCCLIENTCALL_H
#define ASYNCCLIENTCALL_H
#include <memory>

#include <QSharedPointer>

#include <grpc++/grpc++.h>

#include "Proto/roosha_service.pb.h"
#include "Proto/commons.pb.h"

class NetworkManager;
class RooshaServiceConnector;

enum RPCErrorStatus {
    UNKNOWN,
    DEADLINE_EXCEEDED,
    NOT_AUTHENTICATED,
    ALREADY_IN_AUTHNTICATION_PROCESS,
};


/**
 * Base struct for all rpc data. It should be used to hold the all information about call in grpc CompletionQueue.
 */
struct RpcAsyncCall {
    RpcAsyncCall(quint32 id, quint32 timeoutMillis);
    virtual ~RpcAsyncCall();

    virtual void receive(RooshaServiceConnector *connector) = 0;
    virtual void send(RooshaServiceConnector *connector) = 0;
    virtual void succeed(NetworkManager *netManager) = 0;
    virtual void fail(NetworkManager *netManager) = 0;

    const quint32 id_;
    grpc::ClientContext context_;
    grpc::Status status_;
};

/**
 * Base struct for authorization rpc data.
 */
struct AuthorizeOrRegistrateAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    roosha::Credentials request_;
    roosha::AuthenticationToken response_;
};

// ----------------------------------------------------------------------------------------------------

/**
 * This struct represents 'authorize' asynchronous call.
 */
struct AuthorizeAsyncCall : public AuthorizeOrRegistrateAsyncCall {
    using AuthorizeOrRegistrateAsyncCall::AuthorizeOrRegistrateAsyncCall;

    void receive(RooshaServiceConnector *connector) override;
    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    void fail(NetworkManager *netManager) override;

};

/**
 * This struct represents 'registrate' asynchronous call.
 */
struct RegistrateAsyncCall : public AuthorizeOrRegistrateAsyncCall {
    using AuthorizeOrRegistrateAsyncCall::AuthorizeOrRegistrateAsyncCall;

    void receive(RooshaServiceConnector *connector) override;
    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    void fail(NetworkManager *netManager) override;

};


/**
 * This struct represents 'translate' rpc.
 */
struct TranslateAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    void receive(RooshaServiceConnector *connector) override;
    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    void fail(NetworkManager *netManager) override;


    roosha::TranslationRequest request_;
    roosha::Translations response_;
};

/**
 * This struct represents 'proposeUserTransaltions' rpc.
 */
struct ProposeUserTranslationsAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    void receive(RooshaServiceConnector *connector) override;
    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    void fail(NetworkManager *netManager) override;


    roosha::Translations request_;
    roosha::Void response_;
};

#endif // ASYNCCLIENTCALL_H
