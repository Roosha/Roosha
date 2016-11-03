#ifndef ASYNCCLIENTCALL_H
#define ASYNCCLIENTCALL_H
#include <memory>

#include <QSharedPointer>

#include <grpc++/grpc++.h>

#include "Proto/roosha_service.pb.h"
#include "Proto/commons.pb.h"
#include "roosha_service.h"


class ServerResponse {
public:
    ServerResponse(RooshaRpcService *rpcService);

    virtual void process() = 0;

protected:
    friend class RooshaRpcService;

    quint32 id_;
    grpc::ClientContext context_;
    grpc::Status status_;
    RooshaRpcService* rpcService_;
};

class TranslateResponse : public ServerResponse {
public:
    using ServerResponse::ServerResponse;

    void process() override;

private:
    friend class RooshaRpcService;

    roosha::translation::Translations translations_;
};

class ProposeUserTranslationsResponse : public ServerResponse {
public:
    using ServerResponse::ServerResponse;

    void process() override;

private:
    friend class RooshaRpcService;

    roosha::commons::Void response_;
};

class AuthorizeOrRegistrateResponse : public ServerResponse {
public:
    using ServerResponse::ServerResponse;

protected:
    friend class RooshaRpcService;

    roosha::commons::AuthenticationToken token_;
};

class AuthorizeResponse : public AuthorizeOrRegistrateResponse {
public:
    using AuthorizeOrRegistrateResponse::AuthorizeOrRegistrateResponse;

    void process() override;
};

class RegistrateResponse : public AuthorizeOrRegistrateResponse {
public:
    using AuthorizeOrRegistrateResponse::AuthorizeOrRegistrateResponse;

    void process() override;
};

#endif // ASYNCCLIENTCALL_H
