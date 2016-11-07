#ifndef ASYNCCLIENTCALL_H
#define ASYNCCLIENTCALL_H
#include <memory>

#include <QSharedPointer>

#include <grpc++/grpc++.h>

#include "Proto/roosha_service.pb.h"
#include "Proto/commons.pb.h"

class RooshaServiceConnector;

enum RPCErrorStatus {
    UNKNOWN,
    DEADLINE_EXCEEDED,
    EXPIRED_TOKEN,
    AUTHORIZATION_ERROR,
    REGISTRATION_ERROR
};


/**
 * Base struct for all rpc data. It should be used to hold the all information about call in grpc CompletionQueue.
 */
struct RpcAsyncCall {
    RpcAsyncCall(quint32 id, quint32 timeoutMillis);
    virtual ~RpcAsyncCall();

    virtual void receive(RooshaServiceConnector *connector) = 0;

    RPCErrorStatus getStatus();

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
};

/**
 * This struct represents 'registrate' asynchronous call.
 */
struct RegistrateAsyncCall : public AuthorizeOrRegistrateAsyncCall {
    using AuthorizeOrRegistrateAsyncCall::AuthorizeOrRegistrateAsyncCall;

    void receive(RooshaServiceConnector *connector) override;
};


/**
 * This struct represents 'translate' rpc.
 */
struct TranslateAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    void receive(RooshaServiceConnector *connector) override;

    roosha::TranslationRequest request_;
    roosha::Translations response_;
};

/**
 * This struct represents 'proposeUserTransaltions' rpc.
 */
struct ProposeUserTranslationsAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    void receive(RooshaServiceConnector *connector) override;

    roosha::Translations request_;
    roosha::Void response_;
};

#endif // ASYNCCLIENTCALL_H
