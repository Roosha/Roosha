//
// Created by gylo on 04.02.17.
//

#include <QQmlContext>
#include <Core/world.h>

#include "LearningWindowController.h"

LearningWindowController::LearningWindowController(QObject *parent) :
        QObject(parent),
        changesHistoryPosition_((quint32) World::Instance().getCards().size()),
        learningHistoryPosition_((quint32) World::Instance().getLearningHistory().size()),
        strategy_(new SimpleDiffStrategy(World::Instance().getCards().keys(),
                                         QList<Scrutiny>::fromVector(World::Instance().getLearningHistory()),
                                         this)),
        window_(Q_NULLPTR) {

}

void LearningWindowController::registerQmlTypes() {
    static bool registered = false;
    if (!registered) {
        qmlRegisterType<CardDifficulty>(LEARNING_QML_URI, 1, 0, "Difficulty");
        qRegisterMetaType<CardDifficulty::Rate>();

        qmlRegisterType<CardLearningModel>(LEARNING_QML_URI, 1, 0, "CardLearningModel");

        registered = true;
    }

}

void LearningWindowController::showLearningWindow() {
    if (window_) { closeLearningWindow(); }

    updateHistoryInformationForStrategy();

    window_ = new QmlWidget();
    connect(window_, &QmlWidget::closed, this, [this] {
        window_ = Q_NULLPTR;
        strategy_->cancel();
    });

    window_->rootContext()->setContextProperty("controller", this);
    window_->rootContext()->setContextProperty("strategy", strategy_);
    window_->setSource(QStringLiteral("qrc:/learning/LearningWindow.qml"));

    window_->setResizeMode(QQuickWidget::SizeRootObjectToView);
    window_->setAttribute(Qt::WA_DeleteOnClose);
    window_->show();
}

void LearningWindowController::closeLearningWindow() {
    qDebug() << "closeLearningWindow called";
    if (window_) {
        window_->close();
    }
    window_ = Q_NULLPTR;
}

void LearningWindowController::updateHistoryInformationForStrategy() {
    const auto changes = World::Instance().getChanges();
    for (int i = changesHistoryPosition_; i < changes.size(); i++, changesHistoryPosition_++) {
        const auto protoChange = changes[i]->toProtobuf();
        if (protoChange.change_case() == protoChange.kCardChange) {
            if (protoChange.cardchange().change_case() == protoChange.cardchange().kCreateCard) {
                strategy_->onCardCreated(((CreateCard *) changes[i].data())->getCardId());
            } else if (protoChange.cardchange().change_case() == protoChange.cardchange().kDeleteCard) {
                strategy_->onCardDeleted(((DeleteCard *) changes[i].data())->getCardId());
            }
        }
    }

    const auto scrutinies = World::Instance().getLearningHistory();
    auto newScrutinies = scrutinies.mid(learningHistoryPosition_);
    strategy_->appendScrutinies(QList<Scrutiny>::fromVector(newScrutinies));
    learningHistoryPosition_ = (quint32) scrutinies.size();
}
