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


CardListController::CardListController(QObject *parent) : QObject(parent), widget_(nullptr), world_(World::Instance()) {
    auto netManager = ConfigureManager::Instance().getNetworkManager();

    qRegisterMetaType<ChangeList>("ChangeList");
    connect(netManager, &NetworkManager::successLoadChanges,
            this, &CardListController::applyPulledChanges,
            Qt::QueuedConnection);
    // TODO: add translation error handler
}



void CardListController::showCardListWindow() {

    if (widget_) widget_->close();
    widget_ = new QQuickWidget();

    widget_->rootContext()->setContextProperty("cards", QmlConvertation::prepareToQml(world_.getCards()));
    widget_->rootContext()->setContextProperty("controller", this);

    widget_->setSource(QUrl(QStringLiteral("qrc:/list/CardList.qml")));
    widget_->setResizeMode(QQuickWidget::SizeRootObjectToView);

    widget_->setAttribute(Qt::WA_DeleteOnClose);
    connect(widget_, &QQuickWidget::destroyed, [&]() {
        widget_ = Q_NULLPTR;
    });

    widget_->show();

}

void CardListController::applyPulledChanges(quint32 requestId, ChangeList pulledChanges) {
    qDebug("CardListController::applyPulledChanges: pull request %d succeeded", requestId);
    //something useful
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
    ConfigureManager::Instance().getNetworkManager()->loadChanges();
    qDebug() << "pull";
}

void CardListController::pushCards() {
    ConfigureManager::Instance().getNetworkManager()->saveChanges(world_.getChanges());
    qDebug() << "push";
}
