#ifndef TRANSLATIONSERVICE_H
#define TRANSLATIONSERVICE_H

#include "Proto/translation_service.grpc.pb.h"
#include "Proto/translation_service.pb.h"
#include "proto_classes_registration.h" // this header makes it possible to use protobuf classes in signals
#include "async_client_call.h"

#include <grpc/status.h>

#include <memory>
#include <thread>

#include <QString>
#include <QObject>
#include <QThread>

class TranslationService;
class AsyncRpcStatusListener;


class TranslationService : public QObject {
    Q_OBJECT

 public:
    TranslationService(const grpc::string &target);
    ~TranslationService();
 public slots:
    /**
     * Send translation request to translation server request. When the response is received or any error occured,
     * suitable signal(translationSucceeded or translationFailed.
     * @note request ID is not unique across different application launches, but is unique whithin each application
     * launch.
     * @note this function is not thread-safe.
     * @note this function is not blocking.
     *
     * @param source text to be translated.
     * @param timeoutMillis timeout in milliseconds
     * @return ID of request(will be passed to signal emitted as a result of request).
     */
    quint32 translate(QString source, quint32 timeoutMillis);

    /**
     * Send user translation proposal to server.
     * @note this function is not blocking
     *
     * @param userTranslation proposal to be sent to server.
     */
    void proposeUserTranslations(roosha::translation::Translations userTranslations);

 signals:
    /**
     * This signal is emitted when translation request succeeded.
     * @param requestId ID of translation request
     * @param translations server response
     */
    void translationSucceeded(quint32 requestId, roosha::translation::Translations translations);
    /**
     * This signal is emitted when translation request failed.
     * @param requestId ID of translation request
     * @param status grpc status of translation RPC.
     */
    void translationFailed(quint32 requestId, grpc::Status status);
 private:
    friend class AsyncRpcStatusListener;
    void emitTranslationSucceeded(quint32 id, roosha::translation::Translations translations);
    void emitTranslationFailed(quint32 id, grpc::Status status);

    std::unique_ptr<roosha::translation::RooshaTranslationService::Stub> stub_;
    grpc::CompletionQueue completionQueue_;
    //TODO: responseListener_ sould be instance of QThread in order to be able to emit signals!
    //TODO: replace std::thread with QThread.
    AsyncRpcStatusListener* rpcStatusListener_;
};


/**
 * This is utility class intended for internal use in TranslationService class only.
 */
class AsyncRpcStatusListener : public QThread {
    Q_OBJECT

    void run() Q_DECL_OVERRIDE;

 public:
    void setTranslationService(TranslationService* translationService);
 private:
    TranslationService* translationService_;
};

#endif // TRANSLATIONSERVICE_H
