#ifndef CARDLISTCONTROLLER_H
#define CARDLISTCONTROLLER_H

#include <QObject>
#include <QtGlobal>
#include <QMap>
#include <QSharedPointer>
#include <QQuickWidget>
#include "Core/translation.h"
#include "Core/world.h"

class CardListController : public QObject {
    Q_OBJECT
 public:
    CardListController(QObject * parent = nullptr);
    World& world;

    Q_INVOKABLE void closeWindow();
    Q_INVOKABLE void createCard();
signals:
    void createNewCard(QSharedPointer<Translation> data);

 public slots:
    void showCardListWindow();

 private:
    QQuickWidget*  widget;
    //TestTranslations data;
};

#endif // CARDLISTCONTROLLER_H
