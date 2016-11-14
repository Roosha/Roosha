#include "authentication_controller.h"
#include <QQuickWidget>
#include <QQmlContext>
#include <QVariant>
#include <QMap>
#include <QtDebug>
#include <QThread>

#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>

AuthenticationController::AuthenticationController(QObject *parent) : QObject (parent) {
    credentials_ = QVariantMap {
        {"login", "timmy"},
        {"password", "jimmy"}
    };// TODO: get saved credentials from configuration manager
}

void AuthenticationController::showLoginWindow() {
    if(registerWidget_) registerWidget_->close();

    loginWidget_ = new QQuickWidget();

    loginWidget_->rootContext()->setContextProperty("controller", this);
    loginWidget_->rootContext()->setContextProperty("credentials", credentials_);

    loginWidget_->setSource(QUrl(QStringLiteral("qrc:/authentication/loginView.qml")));
    loginWidget_->setResizeMode(QQuickWidget::SizeRootObjectToView);

    loginWidget_->setFixedSize(loginWidget_->size());

    loginWidget_->show();
}

void AuthenticationController::showRegistrateWindow() {
    if(loginWidget_) loginWidget_->close();

    registerWidget_ = new QQuickWidget();

    registerWidget_->rootContext()->setContextProperty("controller", this);

    registerWidget_->setSource(QUrl(QStringLiteral("qrc:/authentication/registerView.qml")));
    registerWidget_->setResizeMode(QQuickWidget::SizeRootObjectToView);

    registerWidget_->setFixedSize(registerWidget_->size());

    registerWidget_->show();
}

void AuthenticationController::sendAuthenticationRequest(QString login, QString password) {
    //TODO: call real networkManager here
    qDebug() << "Authentication with login: " << login << " and password: " << password;
    setState(AuthenticationController::InProgress);
}

void AuthenticationController::sendRegistrateRequest(QString login, QString password) {
    //TODO: call real networkManager here
    qDebug() << "Registrate with login: " << login << " and password: " << password;
    setState(AuthenticationController::InProgress);
}

void AuthenticationController::setState(AuthenticationController::AuthenticationState state) {
    state_ = state;
    emit stateChanged(state_);
}

AuthenticationController::AuthenticationState AuthenticationController::getState() const {
    return state_;
}

void AuthenticationController::authenticationSuccess(quint32 id) {
    setState(AuthenticationController::AuthenticationSuccess);
}

void AuthenticationController::authenticationFail(quint32 id) {
    setState(AuthenticationController::AuthenticationFailure);
}
