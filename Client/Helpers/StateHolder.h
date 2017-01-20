#ifndef ROOSHA_CLIENT_STATEHOLDER_H
#define ROOSHA_CLIENT_STATEHOLDER_H

#include <QtCore/QObject>


class StateHolder : public QObject {
 Q_OBJECT
    //@formatter:off
    Q_PROPERTY(bool auth READ getAuth WRITE setAuth NOTIFY authChanged)
    Q_PROPERTY(qint32 editionWindowOpen READ getEditions WRITE setEditions NOTIFY editionChange)
    Q_PROPERTY(bool sync READ getSync WRITE setSync NOTIFY syncChanged)
    //@formatter:on
 public:
    static StateHolder &Instance();
    bool getAuth() const;
    void setAuth(bool newAuth);
    qint32 getEditions() const;
    void setEditions(qint32 edits);
    bool getSync() const;
    void setSync(bool newSync);

 signals:
    void editionChange();
    void syncChanged();
    void authChanged();

 private:
    StateHolder();
    ~StateHolder();

    StateHolder(const StateHolder &stateHolder) = delete;
    StateHolder &operator=(const StateHolder &) = delete;

    bool auth;
    bool sync;
    qint32 editions;

};

#endif //ROOSHA_CLIENT_STATEHOLDER_H
