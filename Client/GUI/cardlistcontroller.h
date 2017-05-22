#ifndef CARDLISTCONTROLLER_H
#define CARDLISTCONTROLLER_H

#include <QObject>
#include <QtGlobal>
#include <QMap>
#include <QSharedPointer>
#include "GUI/QmlWidget.h"
#include "Core/translation.h"
#include "Core/world.h"
#include "Core/ichange.h"

class CardListController : public QObject {
 Q_OBJECT
 public:
    CardListController(QObject *parent = nullptr);
    World &world_;

    Q_INVOKABLE void closeWindow();
    Q_INVOKABLE void pullCards();
    Q_INVOKABLE void pushCards();
    Q_INVOKABLE void synchronize();
    Q_INVOKABLE void createCard();
    Q_INVOKABLE void deleteCard(QUuid id);
    Q_INVOKABLE void editCard(QUuid id);
    Q_INVOKABLE void learn();

 signals:
    void createNewCard(QSharedPointer<Translation> data);
    void editThisCard(QUuid id);
    void learnCards();
 public slots:
    void showCardListWindow();
    void applySynchronizedChanges(ChangeList synchronizedChanges);

 private:
    QmlWidget *widget_;
};

#endif // CARDLISTCONTROLLER_H
