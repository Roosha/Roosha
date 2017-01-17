#include "cardeditioncontroller.h"
#include <QQmlContext>
#include <QApplication>

CardEditionController::CardEditionController(QObject *parent) : QObject(parent), world(World::Instance()) {
}

void CardEditionController::showNewCardEditWindow(QSharedPointer<Translation> trans) {

    QQuickWidget *editionWidget = new QQuickWidget();
    data = trans;

    editionWidget->rootContext()->setContextProperty("trans", QVariant::fromValue(data.data()));
    editionWidget->rootContext()->setContextProperty("controller", this);
    editionWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    editionWidget->setSource(QUrl(QStringLiteral("qrc:/edition/ShowEdition.qml")));
    editionWidget->show();

    widget = editionWidget;
}

void CardEditionController::showCardEditWindow(QUuid id) {
    QQuickWidget *editionWidget = new QQuickWidget();
    QSharedPointer<DBCard> card = world.getCards().value(id);
    editionWidget->rootContext()->setContextProperty("trans", QVariant::fromValue(card.data()));
    editionWidget->rootContext()->setContextProperty("controller", this);

    editionWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    editionWidget->setSource(QUrl(QStringLiteral("qrc:/edition/ShowEdition.qml")));
    editionWidget->show();

    widget = editionWidget;
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
