#include "cardlistcontroller.h"
#include "Helpers/configuremanager.h"
#include "Network/network_manager.h"

#include <QQmlContext>
#include <QApplication>
#include <Helpers/qmlconvertation.h>
#include <QDebug>
#include <Helpers/StateHolder.h>
#include <QtCore/QThread>
#include <QtCore/QTimer>

CardListController::CardListController(QObject *parent)
        : QObject(parent), world_(World::Instance()), widget_(Q_NULLPTR) {
    auto netManager = ConfigureManager::Instance().getNetworkManager();

    qRegisterMetaType<ChangeList>("ChangeList");
    connect(netManager, &NetworkManager::successLoadChanges,
            this, &CardListController::applyPulledChanges,
            Qt::QueuedConnection);
    // TODO: add translation error handler
}

void CardListController::showCardListWindow() {

    if (widget_) closeWindow();
    widget_ = new QmlWidget();
    connect(widget_, &QmlWidget::closed, this, [this]{ widget_ = Q_NULLPTR;});
    widget_->rootContext()->setContextProperty("cards", QmlConvertation::prepareToQml(world_.getCards()));
    widget_->rootContext()->setContextProperty("controller", this);
    widget_->rootContext()->setContextProperty("stateHolder",  &StateHolder::Instance());
    widget_->setSource(QUrl(QStringLiteral("qrc:/list/CardList.qml")));
    widget_->setResizeMode(QQuickWidget::SizeRootObjectToView);

    widget_->setAttribute(Qt::WA_DeleteOnClose);

    widget_->show();
}

void CardListController::applyPulledChanges(quint32 requestId, ChangeList pulledChanges) {
    qDebug("CardListController::applyPulledChanges: pull request %d succeeded", requestId);

    world_.setChanges(pulledChanges);
    showCardListWindow();
}

void CardListController::closeWindow() {
    widget_->close();
}

void CardListController::createCard() {
    QSharedPointer<Translation> emptyData(new Translation());
    emit createNewCard(emptyData);
}

void CardListController::pullCards() {
    StateHolder::Instance().setSync(true);
    ConfigureManager::Instance().getNetworkManager()->loadChanges();
//    QTimer::singleShot(3000, this, [this]{ StateHolder::Instance().setSync(false); });
    StateHolder::Instance().setSync(false);
    qDebug() << "pull";
}

void CardListController::pushCards() {
    StateHolder::Instance().setSync(true);
    ConfigureManager::Instance().getNetworkManager()->saveChanges(world_.getChanges());
//    QTimer::singleShot(3000, this, [this]{ StateHolder::Instance().setSync(false); });
    StateHolder::Instance().setSync(false);
    qDebug() << "push";
}

void CardListController::deleteCard(QUuid id) {
    world_.deleteCard(id);
    showCardListWindow();
}

void CardListController::editCard(QUuid id) {
    emit editThisCard(id);
}
