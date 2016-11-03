#ifndef ASYNCCLIENTCALL_H
#define ASYNCCLIENTCALL_H
#include <memory>

#include <QSharedPointer>

#include <grpc++/grpc++.h>

#include "Proto/roosha_service.pb.h"
#include "Proto/commons.pb.h"
#include "roosha_service.h"

class AuthenticationManager;

/**
 * Base class for all rpc data. It should be used to hold the all information about call in grpc CompletionQueue.
 */
class RpcAsyncCall {
public:
    // TODO: now rpcService and authManager are not singletones, so we pass them in constructor, but when we make them
    // singletones, this should be altered.
    RpcAsyncCall(RooshaRpcService *rpcService, AuthenticationManager *authManager);
    virtual ~RpcAsyncCall();

    /**
     * General rpcResponse handler. This method should be used whenever response is gotten to trigger appropriate
     * response handler in accordance with the type and status of the response.
     */
    virtual void handle() = 0;

    /**
     * Check whether authentication error received or not.
     * <b>Note:</b> Connection errors or something like that don't make this method return {@code true}
     * @return
     */
    bool isAuthenticationFailed();

protected:
    friend class RooshaRpcService;

    quint32 id_;
    grpc::ClientContext context_;
    grpc::Status status_;
    RooshaRpcService* rpcService_;
    AuthenticationManager* authManager_;
};

/**
 * Base class for technical authorization rpc data.
 */
class AuthorizeOrRegistrateAsyncCall : public RpcAsyncCall {
public:
    using RpcAsyncCall::RpcAsyncCall;
protected:
    friend class RooshaRpcService;

    roosha::commons::Credentials request_;
    roosha::commons::AuthenticationToken response_;
};

/**
 * This class represents 'authorize' asynchronous call.
 */
class AuthorizeAsyncCall : public AuthorizeOrRegistrateAsyncCall {
public:
    using AuthorizeOrRegistrateAsyncCall::AuthorizeOrRegistrateAsyncCall;

    void handle() override;
};

/**
 * This class represents 'registrate' asynchronous call.
 */
class RegistrateAsyncCall : public AuthorizeOrRegistrateAsyncCall {
public:
    using AuthorizeOrRegistrateAsyncCall::AuthorizeOrRegistrateAsyncCall;

    void handle() override;
};

/**
 * Base class for all non-technical authorization calls.
 */
class GeneralPurposeAsyncCall : public RpcAsyncCall { // TODO: come up with more appropriate class name
public:
    using RpcAsyncCall::RpcAsyncCall;

    void handle() override;

    virtual void emitSucceded() = 0;
    virtual void emitFailure() = 0;
    virtual void retry() = 0;
};

/**
 * This class represents 'translate' rpc.
 */
class TranslateAsyncCall : public GeneralPurposeAsyncCall {
public:
    using GeneralPurposeAsyncCall::GeneralPurposeAsyncCall;

    void emitSucceded() override;
    void emitFailure() override;
    void retry() override;

private:
    friend class RooshaRpcService;

    roosha::translation::TranslationRequest request_;
    roosha::translation::Translations response_;
};

/**
 * This class represents 'proposeUserTransaltions' rpc.
 */
class ProposeUserTranslationsAsyncCall : public GeneralPurposeAsyncCall {
public:
    using GeneralPurposeAsyncCall::GeneralPurposeAsyncCall;

    void emitSucceded() override;
    void emitFailure() override;
    void retry() override;

private:
    friend class RooshaRpcService;

    roosha::translation::UserTranslationsProposal request_;
    roosha::commons::Void response_;
};

#endif // ASYNCCLIENTCALL_H
