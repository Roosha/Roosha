//
// Created by gylo on 04.02.17.
//

#ifndef ROOSHA_CLIENT_LEARNINGMANAGER_H
#define ROOSHA_CLIENT_LEARNINGMANAGER_H

#include <QObject>
#include <GUI/QmlWidget.h>
#include "LearningStrategy.h"

#define LEARNING_QML_URI "roosha.learning"

class LearningManager : public QObject {
 Q_OBJECT
 public:
    static void registerQmlTypes();

    LearningManager(QObject *parent = Q_NULLPTR);

    void showLearningWindow();
    Q_INVOKABLE void closeLearningWindow();
 private:
    ILearningStrategy *strategy_;
    QmlWidget *window_;
};

#endif //ROOSHA_CLIENT_LEARNINGMANAGER_H
