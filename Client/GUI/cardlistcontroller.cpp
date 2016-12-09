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


CardListController::CardListController(QObject *parent) : QObject(parent), world_(World::Instance()), widget_(Q_NULLPTR) {
    auto netManager = ConfigureManager::Instance().getNetworkManager();

    qRegisterMetaType<ChangeList>("ChangeList");
    connect(netManager, &NetworkManager::successLoadChanges,
            this, &CardListController::applyPulledChanges,
            Qt::QueuedConnection);
    // TODO: add translation error handler
}



void CardListController::showCardListWindow() {

    if (widget_) {        
        widget_->rootContext()->setContextProperty("cards", QmlConvertation::prepareToQml(world_.getCards()));
        widget_->repaint();
    } else {
        QQuickWidget * listWidget = new QQuickWidget();

        listWidget->rootContext()->setContextProperty("cards", QmlConvertation::prepareToQml(world_.getCards()));
        listWidget->rootContext()->setContextProperty("controller", this);
        listWidget->setAttribute(Qt::WA_DeleteOnClose);
        listWidget->setSource(QUrl(QStringLiteral("qrc:/list/CardList.qml")));
        listWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

        listWidget->show();

        widget_ = listWidget;
        connect(widget_, &QQuickWidget::destroyed, this, &CardListController::onWidgetClose);
    }
}

void CardListController::applyPulledChanges(quint32 requestId, ChangeList pulledChanges) {
    qDebug("CardListController::applyPulledChanges: pull request %d succeeded", requestId);

    world_.setChanges(pulledChanges);
    showCardListWindow();
}

void CardListController::onWidgetClose() {
    widget_ = Q_NULLPTR;
}

void CardListController::closeWindow() {
    widget_->close();
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

void CardListController::deleteCard(QUuid id) {
    world_.deleteCard(id);
    showCardListWindow();
}

void CardListController::editCard(QUuid id) {
    emit editThisCard(id);
}
