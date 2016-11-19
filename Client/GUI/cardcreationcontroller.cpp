#include "cardcreationcontroller.h"
#include "Core/dbcard.h"
#include "Core/world.h"
#include <QQuickWidget>
#include <QQmlContext>
#include <QApplication>
#include <QDesktopWidget>
#include <Helpers/qmlconvertation.h>
#include <QVariant>
#include <QtDebug>

CardCreationController::CardCreationController(QObject *parent) : QObject(parent), world(World::Instance())  {
}


void CardCreationController::showNewEditWindow(QSharedPointer<Translation> trans) {


    QQuickWidget * editionWidget = new QQuickWidget();

    editionWidget->rootContext()->setContextProperty("trans", QVariant::fromValue(trans.data()));
    editionWidget->rootContext()->setContextProperty("controller", this);
    editionWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    editionWidget->setSource(QUrl(QStringLiteral("qrc:/edition/ShowEdition.qml")));
    editionWidget->show();

    widget = editionWidget;
}

void CardCreationController::closeWindow() {
    widget->close();
}

void CardCreationController::saveCard(QString src, QString tarStr, QString exStr) {
    QSharedPointer<DBCard> card = world.createCard();
    QStringList targets = tarStr.split("; ", QString::SkipEmptyParts);
    QStringList examples = exStr.split("; ", QString::SkipEmptyParts);
    card->setSource(src);
    for (int i = 0; i < targets.size(); i++) {
        card->insertElem(TARGET, targets[i], i);
    }
    for (int i = 0; i < examples.size(); i++) {
        card->insertElem(EXAMPLE, examples[i], i);
    }
    qDebug() << src << "\n" << "\n" << tarStr << "\n" << exStr << "\n";

    emit(showCards());
    widget->close();
}
