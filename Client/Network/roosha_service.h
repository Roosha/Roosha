#ifndef TRANSLATIONSERVICE_H
#define TRANSLATIONSERVICE_H

#include "Proto/roosha_service.grpc.pb.h"
#include "Proto/roosha_service.pb.h"

#include <grpc/status.h>

#include <memory>
#include <thread>
#include <atomic>

#include <QString>
#include <QObject>
#include <QThread>

class RooshaRpcService;
class AsyncRpcStatusListener;

class TranslateAsyncCall;
class ProposeUserTranslationsAsyncCall;

class AuthenticationManager;


class RooshaRpcService : public QObject {
    Q_OBJECT

public:
    RooshaRpcService(const grpc::string &target);
    ~RooshaRpcService();
public slots:
    /**
     * Send translation request to translation server request. When the response is received or any error occured,
     * suitable signal(translationSucceeded or translationFailed respectively) will be emitted.
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
     * Send user translation proposal to server. When server responds or any error occurs, suitable
     * signal(userTranslationProposalSecceeded or userTranslationProposalFailed respectively) will be emitted.
     * @note this function is not blocking
     *
     * @param userTranslation proposal to be sent to server.
     * @return ID of request(will be passed to signal emitted as a result of request).
     */
    quint32 proposeUserTranslations(roosha::translation::Translations userTranslations, quint32 timeoutMillis);

    /**
      * Slots with signature fooBarSucceeded(quint32, T) are emitted when a response from server to suitable
      * client request is recieved.
      * Slots with signature fooBarFailed(quint32, grpc::Status) are emitted when a suitable request to server
      * failed(because of whether connection problems or server-side error).
      *
      * @note these signals are emitted in the same background thread each time, so if you use direct connection to
      * these slots, you will block reception of server responses. For further documentation,
      * see http://doc.qt.io/qt-5/qt.html#ConnectionType-enum
      */

signals:
    void translationSucceeded(quint32 requestId, roosha::translation::Translations translations);
    void translationFailed(quint32 requestId, grpc::Status status);
    void userTranslationProposalSucceeded(quint32 requestId, roosha::commons::Void response);
    void userTranslationProposalFailed(quint32 requestId, grpc::Status status);

private:
    friend class AsyncRpcStatusListener;

    friend class TranslateAsyncCall;
    void emitTranslationSucceeded(quint32 id, roosha::translation::Translations translations);
    void emitTranslationFailed(quint32 id, grpc::Status status);
    void sendTranslateRequest(TranslateAsyncCall *call);

    friend class ProposeUserTranslationsAsyncCall;
    void emitUserTranslationProposalSucceeded(quint32 requestId, roosha::commons::Void response);
    void emitUserTranslationProposalFailed(quint32 requestId, grpc::Status status);
    void sendUserTranslationProposal(ProposeUserTranslationsAsyncCall *call);

    std::atomic<quint32> requestIdCount_;
    std::unique_ptr<roosha::translation::RooshaService::Stub> stub_;
    grpc::CompletionQueue completionQueue_;
    AsyncRpcStatusListener* rpcStatusListener_;
    AuthenticationManager* authManager_;

};


/**
 * This is utility class intended for internal use in TranslationService class only.
 */
class AsyncRpcStatusListener : public QThread {
    Q_OBJECT

    void run() Q_DECL_OVERRIDE;

public:
    void setTranslationService(RooshaRpcService* translationService);
private:
    RooshaRpcService* rpcService_;
};

#endif // TRANSLATIONSERVICE_H
