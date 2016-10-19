#ifndef ASYNCCLIENTCALL_H
#define ASYNCCLIENTCALL_H
#include <memory>

#include <QSharedPointer>

#include <grpc++/grpc++.h>

template<typename ResponseType>
struct AsyncClientCall {
    quint32 id_;

    ResponseType response_;

    grpc::ClientContext context_;

    grpc::Status status_;

    std::unique_ptr<grpc::ClientAsyncResponseReader<ResponseType>> responseReader_;
};

#endif // ASYNCCLIENTCALL_H
