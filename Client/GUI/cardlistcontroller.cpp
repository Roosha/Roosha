#include "cardlistcontroller.h"
#include "Helpers/configuremanager.h"
#include "Network/synchronizer.h"
#include "Network/network_manager.h"
#include "cardeditioncontroller.h"
#include "system_tray.h"

#include <QQmlContext>
#include <QApplication>
#include <Helpers/qmlconvertation.h>
#include <Helpers/StateHolder.h>
#include <QtCore/QTimer>

CardListController::CardListController(QObject *parent)
        : QObject(parent), world_(World::Instance()), widget_(Q_NULLPTR) {
    auto netManager = ConfigureManager::Instance().getNetworkManager();
    auto synchronizer = ConfigureManager::Instance().getSynchronizer();

    qRegisterMetaType<ChangeList>("ChangeList");
    connect(synchronizer, &Synchronizer::finishSynchronization,
            this, &CardListController::applySynchronizedChanges,
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

void CardListController::applySynchronizedChanges(ChangeList synchronizedChanges) {
    qDebug("CardListController::applySynchronizedChanges: sync succeeded");

    world_.setChanges(synchronizedChanges);
    showCardListWindow();
}

void CardListController::closeWindow() {
    widget_->close();
}

void CardListController::createCard() {
    QSharedPointer<Translation> emptyData(new Translation());
    emit createNewCard(emptyData);
}

void CardListController::synchronize() {
    StateHolder::Instance().setSync(true);
    ConfigureManager::Instance().getSynchronizer()->synchronize(world_.getChanges());
//    QTimer::singleShot(3000, this, [this]{ StateHolder::Instance().setSync(false); });
    StateHolder::Instance().setSync(false);

}

void CardListController::deleteCard(QUuid id) {
    if (!StateHolder::Instance().isEditing(id)) {
        world_.deleteCard(id);
        showCardListWindow();
    } else
        SystemTray::Notify(QString("Deletion ignored"), QString("You are now editing or learn this card"));
}

void CardListController::editCard(QUuid id) {
    emit editThisCard(id);
}

void CardListController::learn() {
    emit learnCards();
}
