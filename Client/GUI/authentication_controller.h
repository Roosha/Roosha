#ifndef AUTHENTICATIONCONTROLLER_H
#define AUTHENTICATIONCONTROLLER_H

#include <QObject>
#include "GUI/QmlWidget.h"
#include <QVariant>

#include "Helpers/configuremanager.h"

class AuthenticationController : public QObject {
 Q_OBJECT
 public:
    AuthenticationController(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE void showLoginWindow();
    Q_INVOKABLE void showRegistrateWindow();
    Q_INVOKABLE void sendAuthenticationRequest(QString login, QString password);
    Q_INVOKABLE void sendRegistrateRequest(QString login, QString password);

    void closeLoginWindow();
    void closeRegistrateWindow();

    enum AuthenticationState {
        NotAuthenticated,
        InProgress,
        AuthenticationFailure,
        AuthenticationSuccess,
    };
    Q_ENUM(AuthenticationState)

    //@formatter:off
    Q_PROPERTY(AuthenticationState state_ READ getState WRITE setState NOTIFY stateChanged)
    //@formatter:on

    void setState(AuthenticationState state);
    AuthenticationState getState() const;

 public slots:
    void authenticationSuccess(quint32 id);
    void authenticationFail(quint32 id);

 signals:
    void stateChanged(AuthenticationState state);
 private:
    QmlWidget *loginWidget_;
    QmlWidget *registerWidget_;

    QVariantMap credentials_;

    AuthenticationState state_;

    ConfigureManager &configurationManager_;
};

#endif // AUTHENTICATIONCONTROLLER_H
