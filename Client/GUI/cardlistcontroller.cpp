#include "cardlistcontroller.h"
#include "Core/world.h"
#include "Core/translation.h"
#include <QQuickWidget>
#include <QQmlContext>
#include <QApplication>
#include <QDesktopWidget>
#include <Helpers/qmlconvertation.h>
#include <QDebug>


CardListController::CardListController(QObject *parent) : QObject(parent), world_(World::Instance()) { }



void CardListController::showCardListWindow() {

    if (widget) widget->close();
    QQuickWidget * ListWidget = new QQuickWidget();

    ListWidget->rootContext()->setContextProperty("cards", QmlConvertation::prepareToQml(world_.getCards()));
    ListWidget->rootContext()->setContextProperty("controller", this);

    ListWidget->setSource(QUrl(QStringLiteral("qrc:/list/CardList.qml")));
    ListWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    ListWidget->show();

    widget = ListWidget;
}

void CardListController::applyPulledChanges(QVector<QSharedPointer<IChange>> pulledChanges) {
    //something useful
    world_.setChanges(pulledChanges);
    showCardListWindow();
}

void CardListController::closeWindow() {
    widget->close();
}

void CardListController::createCard() {
    QSharedPointer<Translation> emptyData;
    emit createNewCard(emptyData);
}

void CardListController::pullCards() {
    qDebug() << "pull";
}

void CardListController::pushCards() {
    qDebug() << "push";
}
