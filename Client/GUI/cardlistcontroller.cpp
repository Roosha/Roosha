#include "cardlistcontroller.h"
#include "Core/world.h"
#include "Core/translation.h"
#include "Helpers/configuremanager.h"
#include "Network/network_manager.h"

#include <QQuickWidget>
#include <QQmlContext>
#include <QApplication>
#include <QDesktopWidget>
#include <Helpers/qmlconvertation.h>
#include <QDebug>


CardListController::CardListController(QObject *parent) : QObject(parent), world_(World::Instance()) {
    auto netManager = ConfigureManager::Instance().getNetworkManager();

    qRegisterMetaType<ChangeList>("ChangeList");
    connect(netManager, &NetworkManager::successLoadChanges,
            this, &CardListController::applyPulledChanges,
            Qt::QueuedConnection);
    // TODO: add translation error handler
}



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

void CardListController::applyPulledChanges(quint32 requestId, ChangeList pulledChanges) {
    qDebug("CardListController::applyPulledChanges: pull request %d succeeded", requestId);
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
    ConfigureManager::Instance().getNetworkManager()->loadChanges();
    qDebug() << "pull";
}

void CardListController::pushCards() {
    ConfigureManager::Instance().getNetworkManager()->saveChanges(world_.getChanges());
    qDebug() << "push";
}
