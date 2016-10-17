#include "translationcontroller.h"

#include <QQuickWidget>
#include <QQmlContext>
#include <QApplication>
#include <QDesktopWidget>
#include <Helpers/qmlconvertation.h>

TranslationController::TranslationController(QObject *parent) : QObject(parent), lastId(0) {
}

void TranslationController::addData(Translations translations) {
    lastId += 1;

    data.insert(lastId, translations);

    QQuickWidget * translationWidget = new QQuickWidget();

    translationWidget->rootContext()->setContextProperty("trans", QmlConvertation::prepareToQml(data.value(lastId)));
    translationWidget->rootContext()->setContextProperty("controller", this);
    translationWidget->rootContext()->setContextProperty("identificator", lastId);

    translationWidget->setSource(QUrl(QStringLiteral("qrc:/translation/ShowTranslation.qml")));
    translationWidget->setWindowFlags(Qt::FramelessWindowHint);
    translationWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    translationWidget->show();
    translationWidget->move(QApplication::desktop()->screenGeometry().width() - translationWidget->width() - 50, 50);
    translationWidget->setFixedSize(translationWidget->size());

    widgets.insert(lastId, translationWidget);
}

void TranslationController::closeWindow(quint32 id) {
    widgets.value(id)->close();
    widgets.remove(id);
    data.remove(id);
}

void TranslationController::createCard(quint32 id)
{
    // TODO: send signal
}

void TranslationController::createLater(quint32 id)
{
    //TODO: save to special buffer
}
