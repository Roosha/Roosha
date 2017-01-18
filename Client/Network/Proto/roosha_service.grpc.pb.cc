// Generated by the gRPC protobuf plugin.
// If you make any local change, they will be lost.
// source: roosha_service.proto

#include "roosha_service.pb.h"
#include "roosha_service.grpc.pb.h"

#include <grpc++/impl/codegen/async_stream.h>
#include <grpc++/impl/codegen/async_unary_call.h>
#include <grpc++/impl/codegen/channel_interface.h>
#include <grpc++/impl/codegen/client_unary_call.h>
#include <grpc++/impl/codegen/method_handler_impl.h>
#include <grpc++/impl/codegen/rpc_service_method.h>
#include <grpc++/impl/codegen/service_type.h>
#include <grpc++/impl/codegen/sync_stream.h>
namespace roosha {

static const char* RooshaService_method_names[] = {
  "/roosha.RooshaService/knock",
  "/roosha.RooshaService/registrate",
  "/roosha.RooshaService/authorize",
  "/roosha.RooshaService/translate",
  "/roosha.RooshaService/proposeUserTranslations",
  "/roosha.RooshaService/saveChanges",
  "/roosha.RooshaService/loadChanges",
};

std::unique_ptr< RooshaService::Stub> RooshaService::NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options) {
  std::unique_ptr< RooshaService::Stub> stub(new RooshaService::Stub(channel));
  return stub;
}

RooshaService::Stub::Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel)
  : channel_(channel), rpcmethod_knock_(RooshaService_method_names[0], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_registrate_(RooshaService_method_names[1], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_authorize_(RooshaService_method_names[2], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_translate_(RooshaService_method_names[3], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_proposeUserTranslations_(RooshaService_method_names[4], ::grpc::RpcMethod::NORMAL_RPC, channel)
  , rpcmethod_saveChanges_(RooshaService_method_names[5], ::grpc::RpcMethod::CLIENT_STREAMING, channel)
  , rpcmethod_loadChanges_(RooshaService_method_names[6], ::grpc::RpcMethod::SERVER_STREAMING, channel)
  {}

::grpc::Status RooshaService::Stub::knock(::grpc::ClientContext* context, const ::roosha::Void& request, ::roosha::Void* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_knock_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::roosha::Void>* RooshaService::Stub::AsyncknockRaw(::grpc::ClientContext* context, const ::roosha::Void& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::roosha::Void>(channel_.get(), cq, rpcmethod_knock_, context, request);
}

::grpc::Status RooshaService::Stub::registrate(::grpc::ClientContext* context, const ::roosha::Credentials& request, ::roosha::AuthenticationToken* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_registrate_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::roosha::AuthenticationToken>* RooshaService::Stub::AsyncregistrateRaw(::grpc::ClientContext* context, const ::roosha::Credentials& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::roosha::AuthenticationToken>(channel_.get(), cq, rpcmethod_registrate_, context, request);
}

::grpc::Status RooshaService::Stub::authorize(::grpc::ClientContext* context, const ::roosha::Credentials& request, ::roosha::AuthenticationToken* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_authorize_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::roosha::AuthenticationToken>* RooshaService::Stub::AsyncauthorizeRaw(::grpc::ClientContext* context, const ::roosha::Credentials& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::roosha::AuthenticationToken>(channel_.get(), cq, rpcmethod_authorize_, context, request);
}

::grpc::Status RooshaService::Stub::translate(::grpc::ClientContext* context, const ::roosha::TranslationRequest& request, ::roosha::Translations* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_translate_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::roosha::Translations>* RooshaService::Stub::AsynctranslateRaw(::grpc::ClientContext* context, const ::roosha::TranslationRequest& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::roosha::Translations>(channel_.get(), cq, rpcmethod_translate_, context, request);
}

::grpc::Status RooshaService::Stub::proposeUserTranslations(::grpc::ClientContext* context, const ::roosha::Translations& request, ::roosha::Void* response) {
  return ::grpc::BlockingUnaryCall(channel_.get(), rpcmethod_proposeUserTranslations_, context, request, response);
}

::grpc::ClientAsyncResponseReader< ::roosha::Void>* RooshaService::Stub::AsyncproposeUserTranslationsRaw(::grpc::ClientContext* context, const ::roosha::Translations& request, ::grpc::CompletionQueue* cq) {
  return new ::grpc::ClientAsyncResponseReader< ::roosha::Void>(channel_.get(), cq, rpcmethod_proposeUserTranslations_, context, request);
}

::grpc::ClientWriter< ::roosha::Change>* RooshaService::Stub::saveChangesRaw(::grpc::ClientContext* context, ::roosha::Void* response) {
  return new ::grpc::ClientWriter< ::roosha::Change>(channel_.get(), rpcmethod_saveChanges_, context, response);
}

::grpc::ClientAsyncWriter< ::roosha::Change>* RooshaService::Stub::AsyncsaveChangesRaw(::grpc::ClientContext* context, ::roosha::Void* response, ::grpc::CompletionQueue* cq, void* tag) {
  return new ::grpc::ClientAsyncWriter< ::roosha::Change>(channel_.get(), cq, rpcmethod_saveChanges_, context, response, tag);
}

::grpc::ClientReader< ::roosha::Change>* RooshaService::Stub::loadChangesRaw(::grpc::ClientContext* context, const ::roosha::Void& request) {
  return new ::grpc::ClientReader< ::roosha::Change>(channel_.get(), rpcmethod_loadChanges_, context, request);
}

::grpc::ClientAsyncReader< ::roosha::Change>* RooshaService::Stub::AsyncloadChangesRaw(::grpc::ClientContext* context, const ::roosha::Void& request, ::grpc::CompletionQueue* cq, void* tag) {
  return new ::grpc::ClientAsyncReader< ::roosha::Change>(channel_.get(), cq, rpcmethod_loadChanges_, context, request, tag);
}

RooshaService::Service::Service() {
  (void)RooshaService_method_names;
  AddMethod(new ::grpc::RpcServiceMethod(
      RooshaService_method_names[0],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< RooshaService::Service, ::roosha::Void, ::roosha::Void>(
          std::mem_fn(&RooshaService::Service::knock), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RooshaService_method_names[1],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< RooshaService::Service, ::roosha::Credentials, ::roosha::AuthenticationToken>(
          std::mem_fn(&RooshaService::Service::registrate), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RooshaService_method_names[2],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< RooshaService::Service, ::roosha::Credentials, ::roosha::AuthenticationToken>(
          std::mem_fn(&RooshaService::Service::authorize), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RooshaService_method_names[3],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< RooshaService::Service, ::roosha::TranslationRequest, ::roosha::Translations>(
          std::mem_fn(&RooshaService::Service::translate), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RooshaService_method_names[4],
      ::grpc::RpcMethod::NORMAL_RPC,
      new ::grpc::RpcMethodHandler< RooshaService::Service, ::roosha::Translations, ::roosha::Void>(
          std::mem_fn(&RooshaService::Service::proposeUserTranslations), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RooshaService_method_names[5],
      ::grpc::RpcMethod::CLIENT_STREAMING,
      new ::grpc::ClientStreamingHandler< RooshaService::Service, ::roosha::Change, ::roosha::Void>(
          std::mem_fn(&RooshaService::Service::saveChanges), this)));
  AddMethod(new ::grpc::RpcServiceMethod(
      RooshaService_method_names[6],
      ::grpc::RpcMethod::SERVER_STREAMING,
      new ::grpc::ServerStreamingHandler< RooshaService::Service, ::roosha::Void, ::roosha::Change>(
          std::mem_fn(&RooshaService::Service::loadChanges), this)));
}

RooshaService::Service::~Service() {
}

::grpc::Status RooshaService::Service::knock(::grpc::ServerContext* context, const ::roosha::Void* request, ::roosha::Void* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RooshaService::Service::registrate(::grpc::ServerContext* context, const ::roosha::Credentials* request, ::roosha::AuthenticationToken* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RooshaService::Service::authorize(::grpc::ServerContext* context, const ::roosha::Credentials* request, ::roosha::AuthenticationToken* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RooshaService::Service::translate(::grpc::ServerContext* context, const ::roosha::TranslationRequest* request, ::roosha::Translations* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RooshaService::Service::proposeUserTranslations(::grpc::ServerContext* context, const ::roosha::Translations* request, ::roosha::Void* response) {
  (void) context;
  (void) request;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RooshaService::Service::saveChanges(::grpc::ServerContext* context, ::grpc::ServerReader< ::roosha::Change>* reader, ::roosha::Void* response) {
  (void) context;
  (void) reader;
  (void) response;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}

::grpc::Status RooshaService::Service::loadChanges(::grpc::ServerContext* context, const ::roosha::Void* request, ::grpc::ServerWriter< ::roosha::Change>* writer) {
  (void) context;
  (void) request;
  (void) writer;
  return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
}


}  // namespace roosha

