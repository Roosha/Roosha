#include "cardlistcontroller.h"
#include "Core/world.h"
#include "Core/translation.h"
#include <QQuickWidget>
#include <QQmlContext>
#include <QApplication>
#include <QDesktopWidget>
#include <Helpers/qmlconvertation.h>
#include <QDebug>


CardListController::CardListController(QObject *parent) : QObject(parent), world(World::Instance()) { }



void CardListController::showCardListWindow() {

    if (widget) widget->close();
    QQuickWidget * ListWidget = new QQuickWidget();

    ListWidget->rootContext()->setContextProperty("cards", QmlConvertation::prepareToQml(world.getCards()));
    ListWidget->rootContext()->setContextProperty("controller", this);

    ListWidget->setSource(QUrl(QStringLiteral("qrc:/list/CardList.qml")));
    ListWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    ListWidget->show();
    ListWidget->move(QApplication::desktop()->screenGeometry().width() / 2,  QApplication::desktop()->screenGeometry().height() / 2);

    widget = ListWidget;
}

void CardListController::closeWindow() {
    widget->close();
}

void CardListController::createCard() {
    QSharedPointer<Translation> emptyData;
    emit createNewCard(emptyData);
}
