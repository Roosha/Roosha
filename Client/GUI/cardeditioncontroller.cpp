#include "cardeditioncontroller.h"
#include <QQmlContext>
#include <QApplication>
#include <Helpers/StateHolder.h>
#include <QDebug>
CardEditionController::CardEditionController(QObject *parent) : QObject(parent), world(World::Instance()) {
}

void CardEditionController::showNewCardEditWindow(QSharedPointer<Translation> trans) {

    QmlWidget *editionWidget = new QmlWidget();

    data_.insert(editionWidget, trans);

    editionWidget->rootContext()->setContextProperty("trans", QVariant::fromValue(data_[editionWidget].data()));
    editionWidget->rootContext()->setContextProperty("controller", this);
    editionWidget->rootContext()->setContextProperty("self", editionWidget);
    editionWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    editionWidget->setSource(QUrl(QStringLiteral("qrc:/edition/ShowEdition.qml")));
    editionWidget->show();

    StateHolder::Instance().setEditions(data_.size());

    connect(editionWidget, &QmlWidget::closed, this,
            [this, editionWidget] {
                data_.remove(editionWidget);
                StateHolder::Instance().setEditions(data_.size());
            });
}

void CardEditionController::showCardEditWindow(QUuid id) {
    QmlWidget *editionWidget = new QmlWidget();
    QSharedPointer<DBCard> card = world.getCards().value(id);
    editionWidget->rootContext()->setContextProperty("trans", QVariant::fromValue(card.data()));
    editionWidget->rootContext()->setContextProperty("controller", this);
    editionWidget->rootContext()->setContextProperty("self", editionWidget);
    editionWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    editionWidget->setSource(QUrl(QStringLiteral("qrc:/edition/ShowEdition.qml")));
    editionWidget->show();

    StateHolder::Instance().setEditions(data_.size());

    connect(editionWidget, &QmlWidget::closed, this,
            [this, editionWidget] {
                data_.remove(editionWidget);
                StateHolder::Instance().setEditions(data_.size());
                qInfo() << "size of data " << data_.size();
            });
}

void CardEditionController::closeWindow(QQuickWidget *widget) {
    widget->close();
}

void CardEditionController::saveCard(QQuickWidget *widget, QString src, QString tarStr, QString exStr, QUuid id) {
    QSharedPointer<DBCard> card = id.isNull() ? world.createCard() : world.getCards().value(id);

    QStringList targets = tarStr.split("; ", QString::SkipEmptyParts);
    QStringList examples = exStr.split(";\n", QString::SkipEmptyParts);
    card->setSource(src);
    card->setField(TARGET, targets);
    card->setField(EXAMPLE, examples);

    emit(showCards());
    widget->close();
}
