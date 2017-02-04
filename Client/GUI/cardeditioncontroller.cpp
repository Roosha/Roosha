#include "cardeditioncontroller.h"
#include <QQmlContext>
#include <QApplication>
#include <Helpers/StateHolder.h>
#include <QDebug>
CardEditionController::CardEditionController(QObject *parent) : QObject(parent), world(World::Instance()) {
}

void CardEditionController::showNewCardEditWindow(QSharedPointer<Translation> trans) {

    QmlWidget *editionWidget = data_create.key(trans, nullptr);
    if (!editionWidget) {
        editionWidget = new QmlWidget();

        data_create.insert(editionWidget, trans);

        editionWidget->rootContext()->setContextProperty("trans", QVariant::fromValue(data_create[editionWidget].data()));
        editionWidget->rootContext()->setContextProperty("controller", this);
        editionWidget->rootContext()->setContextProperty("self", editionWidget);
        editionWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        editionWidget->setSource(QUrl(QStringLiteral("qrc:/edition/ShowEdition.qml")));
        editionWidget->show();

        StateHolder::Instance().setEditors(data_create.size() + data_edit.size());

        connect(editionWidget, &QmlWidget::closed, this,
                [this, editionWidget] {
                    data_create.remove(editionWidget);
                    StateHolder::Instance().setEditors(data_create.size() + data_edit.size());
                });
    } else
        editionWidget->activateWindow();
}

void CardEditionController::showCardEditWindow(QUuid id) {

    QmlWidget *editionWidget = data_edit.key(id, nullptr);

    if (!editionWidget) {
        editionWidget = new QmlWidget();
        QSharedPointer<DBCard> card = world.getCards().value(id);
        data_edit.insert(editionWidget, id);

        editionWidget->rootContext()->setContextProperty("trans", QVariant::fromValue(card.data()));
        editionWidget->rootContext()->setContextProperty("controller", this);
        editionWidget->rootContext()->setContextProperty("self", editionWidget);
        editionWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
        editionWidget->setSource(QUrl(QStringLiteral("qrc:/edition/ShowEdition.qml")));
        editionWidget->show();

        StateHolder::Instance().setEditors(data_create.size() + data_edit.size());
        StateHolder::Instance().startEdit(id);

        connect(editionWidget, &QmlWidget::closed, this,
                [this, editionWidget, id] {
                    data_edit.remove(editionWidget);
                    StateHolder::Instance().setEditors(data_create.size() + data_edit.size());
                    StateHolder::Instance().endEdit(id);
                });
    } else
        editionWidget->activateWindow();
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
