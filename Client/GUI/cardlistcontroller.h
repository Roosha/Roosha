#ifndef CARDLISTCONTROLLER_H
#define CARDLISTCONTROLLER_H

#include <QObject>
#include <QtGlobal>
#include <QMap>
#include <QSharedPointer>
#include <QQuickWidget>
#include "Core/translation.h"
#include "Core/world.h"
#include "Core/ichange.h"

class CardListController : public QObject {
    Q_OBJECT
public:
    CardListController(QObject * parent = nullptr);
    World& world_;

    Q_INVOKABLE void closeWindow();
    Q_INVOKABLE void pullCards();
    Q_INVOKABLE void pushCards();
    Q_INVOKABLE void createCard();

signals:
    void createNewCard(QSharedPointer<Translation> data);

public slots:
    void showCardListWindow();
    void applyPulledChanges(QVector<QSharedPointer<IChange>> pulledChanges);

private:
    QQuickWidget*  widget;
    //Translations data;
};

#endif // CARDLISTCONTROLLER_H
