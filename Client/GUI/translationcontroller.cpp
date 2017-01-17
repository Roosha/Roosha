#include "translationcontroller.h"

#include <QQmlContext>
#include <QApplication>
#include <QDesktopWidget>
#include <Helpers/qmlconvertation.h>
TranslationController::TranslationController(QObject *parent) : QObject(parent) {
}

void TranslationController::addData(Translations translations) {
    QQuickWidget *translationWidget = new QQuickWidget();

    data.insert(translationWidget, translations);

    translationWidget->rootContext()->setContextProperty("trans",
                                                         QmlConvertation::prepareToQml(data.value(translationWidget)));
    translationWidget->rootContext()->setContextProperty("controller", this);
    translationWidget->rootContext()->setContextProperty("self", translationWidget);

    translationWidget->setSource(QUrl(QStringLiteral("qrc:/translation/ShowTranslation.qml")));
    translationWidget->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    translationWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);

//    translationWidget->setAttribute(Qt::WA_DeleteOnClose);

    translationWidget->show();
//    translationWidget->move(QApplication::desktop()->screenGeometry().width() - translationWidget->width() - 50, 50);
    replaceWindows();
    translationWidget->setFixedSize(translationWidget->size());
}

void TranslationController::closeWindow(QQuickWidget *widget) {
    widget->close();
    data.remove(widget);
    replaceWindows();
}

void TranslationController::createCard(QQuickWidget *widget, int index) {
    emit createNewCard(data[widget][index]);
}

void TranslationController::replaceWindows() {
    int absTopMargin = 50;
    int spacing = 20;

    int number = 0;
    for (auto widget : data.keys()) {
        int yPos = absTopMargin + (widget->height() + spacing) * (number++);
        int xPos = QApplication::desktop()->screenGeometry().width() - widget->width() - 50;
        widget->move(xPos, yPos);
    }
}

//void TranslationController::createLater(quint32 id)
//{
//    //TODO: save to special buffer
//}
