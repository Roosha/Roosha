#include "cardeditioncontroller.h"
#include <QQmlContext>
#include <QApplication>

CardEditionController::CardEditionController(QObject *parent) : QObject(parent), world(World::Instance()) {
}

void CardEditionController::showNewCardEditWindow(QSharedPointer<Translation> trans) {

    QmlWidget *editionWidget = new QmlWidget();
    data = trans;

    editionWidget->rootContext()->setContextProperty("trans", QVariant::fromValue(data.data()));
    editionWidget->rootContext()->setContextProperty("controller", this);
    editionWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    editionWidget->setSource(QUrl(QStringLiteral("qrc:/edition/ShowEdition.qml")));
    editionWidget->show();

    widget = editionWidget;

    connect(widget, &QmlWidget::closed, this, [this]{widget = Q_NULLPTR; });
}

void CardEditionController::showCardEditWindow(QUuid id) {
    QmlWidget *editionWidget = new QmlWidget();
    QSharedPointer<DBCard> card = world.getCards().value(id);
    editionWidget->rootContext()->setContextProperty("trans", QVariant::fromValue(card.data()));
    editionWidget->rootContext()->setContextProperty("controller", this);

    editionWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    editionWidget->setSource(QUrl(QStringLiteral("qrc:/edition/ShowEdition.qml")));
    editionWidget->show();

    widget = editionWidget;

    connect(widget, &QmlWidget::closed, this, [this]{widget = Q_NULLPTR;});
}

void CardEditionController::closeWindow() {
    widget->close();
}

void CardEditionController::saveCard(QString src, QString tarStr, QString exStr, QUuid id) {
    QSharedPointer<DBCard> card = id.isNull() ? world.createCard() : world.getCards().value(id);

    QStringList targets = tarStr.split("; ", QString::SkipEmptyParts);
    QStringList examples = exStr.split(";\n", QString::SkipEmptyParts);
    card->setSource(src);
    card->setField(TARGET, targets);
    card->setField(EXAMPLE, examples);

    emit(showCards());
    widget->close();
}
