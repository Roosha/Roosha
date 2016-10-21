#ifndef ASYNCCLIENTCALL_H
#define ASYNCCLIENTCALL_H
#include <memory>

#include <QSharedPointer>

#include <grpc++/grpc++.h>

#include "Proto/translation_service.pb.h"
#include "Proto/commons.pb.h"

enum RequestType {
    // response type: roosha::translation::Translations
    TRANSLATE,
    // response type: roosha::commons::Void
    PROPOSE_USER_TRANSLATION,
};

struct AsyncClientCall {
    ~AsyncClientCall() {
        if (response_ == nullptr) {
            return;
        }
        switch(requestType_) {
            case TRANSLATE:
                delete static_cast<roosha::translation::Translations*>(response_);
                break;
            case PROPOSE_USER_TRANSLATION:
                delete static_cast<roosha::commons::Void*>(response_);
                break;
        }
    }

    quint32 id_;

    RequestType requestType_;

    // ResponseType* response_;
    void* response_;

    grpc::ClientContext context_;

    grpc::Status status_;

};

#endif // ASYNCCLIENTCALL_H
