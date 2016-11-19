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

    if (widget_) {
        widget_->rootContext()->setContextProperty("cards", QmlConvertation::prepareToQml(world_.getCards()));
        widget_->repaint();
    }    else {
        QQuickWidget * listWidget = new QQuickWidget();

        listWidget->rootContext()->setContextProperty("cards", QmlConvertation::prepareToQml(world_.getCards()));
        listWidget->rootContext()->setContextProperty("controller", this);

        listWidget->setSource(QUrl(QStringLiteral("qrc:/list/CardList.qml")));
        listWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

        listWidget->show();

        widget_ = listWidget;
    }
}

void CardListController::applyPulledChanges(QVector<QSharedPointer<IChange>> pulledChanges) {
    world_.setChanges(pulledChanges);
    showCardListWindow();
}

void CardListController::closeWindow() {
    widget_->close();
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

void CardListController::deleteCard(QUuid id) {
    world_.deleteCard(id);
    showCardListWindow();
}
