//
// Created by gylo on 04.02.17.
//

#include <QQmlContext>
#include <Core/world.h>

#include "LearningManager.h"

LearningManager::LearningManager(QObject *parent) :
        QObject(parent),
        strategy_(new TestStrategy(this)),
        window_(Q_NULLPTR) {

}

void LearningManager::showLearningWindow() {
    if (window_) { closeLearningWindow(); }

    window_ = new QmlWidget();
    connect(window_, &QmlWidget::closed, this, [this] { window_ = Q_NULLPTR; });

    window_->setSource(QStringLiteral("qrc:/learning/LearningWindow.qml"));

    window_->rootContext()->setContextProperty("controller", this);
    window_->rootContext()->setContextProperty("strategy", strategy_);

    window_->setResizeMode(QQuickWidget::SizeRootObjectToView);
    window_->setAttribute(Qt::WA_DeleteOnClose);
    window_->show();
}

void LearningManager::closeLearningWindow() {
    qDebug() << "closeLearningWindow called";
    if (window_) {
        window_->close();
    }
    window_ = Q_NULLPTR;
}
