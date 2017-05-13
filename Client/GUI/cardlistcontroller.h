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
    Q_INVOKABLE void createCard();
    Q_INVOKABLE void deleteCard(QUuid id);
    Q_INVOKABLE void editCard(QUuid id);
    Q_INVOKABLE void learn();

    static CardListController *getInstance() {
        if(self == nullptr) {
            self = new CardListController();
        }
        return self;
    }

 signals:
    void createNewCard(QSharedPointer<Translation> data);
    void editThisCard(QUuid id);
    void learnCards();
 public slots:
    void showCardListWindow();
    void applyPulledChanges(quint32 requestId, ChangeList pulledChanges);

 private:
    QmlWidget *widget_;

    static CardListController *self;
};


static QObject *register_card_list_controller(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return CardListController::getInstance();
}

#endif // CARDLISTCONTROLLER_H
