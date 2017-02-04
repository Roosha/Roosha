#ifndef ROOSHA_CLIENT_STATEHOLDER_H
#define ROOSHA_CLIENT_STATEHOLDER_H

#include <QtCore/QObject>
#include <QtCore/QSet>
#include <QtCore/QUuid>

class StateHolder : public QObject {
 Q_OBJECT
    //@formatter:off
    Q_PROPERTY(bool auth READ isAuth WRITE setAuth NOTIFY authChanged)
    Q_PROPERTY(qint32 editionWindowOpen READ getEditors WRITE setEditors NOTIFY editionChange)
    Q_PROPERTY(bool sync READ isSync WRITE setSync NOTIFY syncChanged)
    //@formatter:on
 public:
    static StateHolder &Instance();
    bool isAuth() const;
    void setAuth(bool newAuth);
    qint32 getEditors() const;
    void setEditors(qint32 edits);
    bool isSync() const;
    void setSync(bool newSync);
    bool isEditing(QUuid id);
    void startEdit(QUuid id);
    void endEdit(QUuid id);
 signals:
    void editionChange();
    void syncChanged();
    void authChanged();

 private:
    StateHolder();
    ~StateHolder();

    StateHolder(const StateHolder &stateHolder) = delete;
    StateHolder &operator=(const StateHolder &) = delete;

    bool isAuthorized;
    bool synchronizationInProgress;
    qint32 editors;
    QSet<QUuid> editingCards;
};

#endif //ROOSHA_CLIENT_STATEHOLDER_H
