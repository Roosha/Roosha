#include "translationcontroller.h"

#include <QQuickWidget>
#include <QQmlContext>
#include <Helpers/qmlconvertation.h>

TranslationController::TranslationController(QObject *parent) : QObject(parent) {

}

void TranslationController::setData(Translations data) { // TODO: change to addData in order to show several translation in the same moment
    this->data = data;
}

void TranslationController::show() {
    QQuickWidget * translationWidget = new QQuickWidget();
    translationWidget->rootContext()->setContextProperty("trans", QmlConvertation::prepareToQml(data));

    translationWidget->setSource(QUrl(QStringLiteral("qrc:/showTranslation.qml")));
    translationWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

    translationWidget->show();
}

void TranslationController::clearData() {
    data.clear();
}
