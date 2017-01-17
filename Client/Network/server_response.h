#ifndef ASYNCCLIENTCALL_H
#define ASYNCCLIENTCALL_H
#include <memory>

#include <QSharedPointer>
#include <QList>

#include <grpc++/grpc++.h>

#include <Core/ichange.h>
#include "Proto/roosha_service.pb.h"
#include "Proto/commons.pb.h"

class NetworkManager;
class RooshaServiceConnector;
class AuthenticationManager;

enum RPCErrorStatus {
    UNKNOWN,
    DEADLINE_EXCEEDED,
    NOT_AUTHENTICATED,
    ALREADY_IN_AUTHNTICATION_PROCESS,

};

Q_DECLARE_METATYPE(RPCErrorStatus)

/**
 * Base struct for all rpc data. It should be used to hold the all information about call in grpc CompletionQueue.
 */
struct RpcAsyncCall {
    RpcAsyncCall(quint32 id, quint32 timeoutMillis);
    virtual ~RpcAsyncCall();

    /**
     * These methods provide interface for simple passage of any RpcAsyncCall through network stack.
     * Each method just passes this call to proper handler on appropriate network stack layer:
     * This is the cheme of request performance:
     *
     * CLIENT              NetworkManager            AuthManager      Connector        Listener           REMOTE SERVER
     *
     *        CLIENT CODE               authenticate           send                  GRPC GENERATED CODE
     *    =====================>    ===================>    ===========>    ====================================>
     *
     *        Qt SIGNAL                 succeed/fail           verify          receive       GRPC GENERATED CODE
     *    <=====================    <===================    <===========    <===========    <====================
     */

    virtual void authenticate(AuthenticationManager *authManager) = 0;
    virtual void send(RooshaServiceConnector *connector) = 0;
    void receive(RooshaServiceConnector *connector);
    virtual void verify(AuthenticationManager *authManager) = 0;

    /**
     * NOTE: this method will destroy this RpcAsyncCall instance, so do not deal with this object anymore after call.
     */
    virtual void succeed(NetworkManager *netManager) = 0;

    /**
     * NOTE: this method will destroy this RpcAsyncCall instance, so do not deal with this object anymore after call.
     */
    void fail(NetworkManager *netManager);

    /**
     * NOTE: this method will destroy this RpcAsyncCall instance, so do not deal with this object anymore after call.
     */
    virtual void fail(NetworkManager *netManager, RPCErrorStatus status) = 0;

    const quint32 id_;
    grpc::ClientContext context_;
    grpc::Status status_;
};

/**
 * Base struct for authorization rpc data.
 */
struct AuthorizeOrRegistrateAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    void authenticate(AuthenticationManager *authManager) override;
    void verify(AuthenticationManager *authManager) override;

    roosha::Credentials request_;
    roosha::AuthenticationToken response_;
};

// ----------------------------------------------------------------------------------------------------

/**
 * This struct represents 'authorize' asynchronous call.
 */
struct AuthorizeAsyncCall : public AuthorizeOrRegistrateAsyncCall {
    using AuthorizeOrRegistrateAsyncCall::AuthorizeOrRegistrateAsyncCall;

    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    using RpcAsyncCall::fail;
    void fail(NetworkManager *netManager, RPCErrorStatus status) override;
};

/**
 * This struct represents 'registrate' asynchronous call.
 */
struct RegistrateAsyncCall : public AuthorizeOrRegistrateAsyncCall {
    using AuthorizeOrRegistrateAsyncCall::AuthorizeOrRegistrateAsyncCall;

    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    using RpcAsyncCall::fail;
    void fail(NetworkManager *netManager, RPCErrorStatus status) override;
};

// ----------------------------------------------------------------------------------------------------

/**
 * This struct represents any rpc which requires authentication.
 */
struct AuthenticatedAsyncCall : public RpcAsyncCall {
    using RpcAsyncCall::RpcAsyncCall;

    void authenticate(AuthenticationManager *authManager) override;
    void verify(AuthenticationManager *authManager) override;

    virtual bool isAuthenticationRequired();
};

/**
 * This struct represents 'translate' rpc.
 */
struct TranslateAsyncCall : public AuthenticatedAsyncCall {
    using AuthenticatedAsyncCall::AuthenticatedAsyncCall;

    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    using RpcAsyncCall::fail;
    void fail(NetworkManager *netManager, RPCErrorStatus status) override;

    bool isAuthenticationRequired() override;

    roosha::TranslationRequest request_;
    roosha::Translations response_;
};

/**
 * This struct represents 'proposeUserTransaltions' rpc.
 */
struct ProposeUserTranslationsAsyncCall : public AuthenticatedAsyncCall {
    using AuthenticatedAsyncCall::AuthenticatedAsyncCall;

    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    using RpcAsyncCall::fail;
    void fail(NetworkManager *netManager, RPCErrorStatus status) override;

    roosha::Translations request_;
    roosha::Void response_;
};

struct SaveChangesAsyncCall : public AuthenticatedAsyncCall {
    using AuthenticatedAsyncCall::AuthenticatedAsyncCall;

    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    using RpcAsyncCall::fail;
    void fail(NetworkManager *netManager, RPCErrorStatus status) override;

    QList<roosha::Change> request_;
    roosha::Void response_;
};

struct LoadChangesAsyncCall : public AuthenticatedAsyncCall {
    using AuthenticatedAsyncCall::AuthenticatedAsyncCall;

    void send(RooshaServiceConnector *connector) override;
    void succeed(NetworkManager *netManager) override;
    using RpcAsyncCall::fail;
    void fail(NetworkManager *netManager, RPCErrorStatus status) override;

    roosha::Void request_;
    ChangeList response_;
};

#endif // ASYNCCLIENTCALL_H
