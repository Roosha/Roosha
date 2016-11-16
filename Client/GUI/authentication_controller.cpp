#include "authentication_controller.h"
#include <QQuickWidget>
#include <QQmlContext>
#include <QVariant>
#include <QMap>
#include <QtDebug>
#include <QThread>

#include "Network/network_manager.h"

AuthenticationController::AuthenticationController(QObject *parent) :
    QObject (parent),
    loginWidget_(nullptr),
    registerWidget_(nullptr),
    configurationManager_(ConfigureManager::Instance()) {

    qRegisterMetaType<AuthenticationState>("AuthenticationState");

    credentials_ = QVariantMap {
        {"login", configurationManager_.getLogin()},
        {"password", configurationManager_.getPasswordHash()}
    };
}

void AuthenticationController::showLoginWindow() {
    setState(AuthenticationController::NotAuthenticated);
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
    setState(AuthenticationController::NotAuthenticated);
    if(loginWidget_) loginWidget_->close();

    registerWidget_ = new QQuickWidget();

    registerWidget_->rootContext()->setContextProperty("controller", this);

    registerWidget_->setSource(QUrl(QStringLiteral("qrc:/authentication/registerView.qml")));
    registerWidget_->setResizeMode(QQuickWidget::SizeRootObjectToView);

    registerWidget_->setFixedSize(registerWidget_->size());

    registerWidget_->show();
}

void AuthenticationController::sendAuthenticationRequest(QString login, QString password) {
    setState(AuthenticationController::InProgress);
    configurationManager_.getNetworkManager()->authorize(login, password);
}

void AuthenticationController::sendRegistrateRequest(QString login, QString password) {
    setState(AuthenticationController::InProgress);
    configurationManager_.getNetworkManager()->registrate(login, password);
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
    if(registerWidget_) registerWidget_->close();
    if(loginWidget_) loginWidget_->close();
}

void AuthenticationController::authenticationFail(quint32 id) {
    if(loginWidget_) {
        setState(AuthenticationController::AuthenticationFailure);
    } else {
        showLoginWindow();
    }
}
