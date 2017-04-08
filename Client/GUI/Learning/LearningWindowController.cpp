//
// Created by gylo on 04.02.17.
//

#include <QQmlContext>
#include <Core/world.h>

#include "LearningWindowController.h"

LearningWindowController::LearningWindowController(QObject *parent) :
        QObject(parent),
        strategy_(new RandomCardStrategy(this)),
        window_(Q_NULLPTR) {

}

void LearningWindowController::registerQmlTypes() {
    static bool registered = false;
    if (!registered) {
        qmlRegisterType<CardDifficulty>(LEARNING_QML_URI, 1, 0, "Difficulty");
        qRegisterMetaType<CardDifficulty::Rate>();

        qmlRegisterType<CardLearningModel>();
        qmlRegisterType<CardViewModelBase>();
        qmlRegisterType<UserInputModelBase>();

        registered = true;
    }

}

void LearningWindowController::showLearningWindow() {
    if (window_) { closeLearningWindow(); }

    window_ = new QmlWidget();
    connect(window_, &QmlWidget::closed, this, [this] { window_ = Q_NULLPTR; strategy_->cancel(); });

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