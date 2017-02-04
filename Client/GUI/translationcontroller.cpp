#include "translationcontroller.h"
#include "system_tray.h"

#include <QQmlContext>
#include <QApplication>
#include <QDesktopWidget>
#include <Helpers/qmlconvertation.h>
#include <Helpers/StateHolder.h>
TranslationController::TranslationController(QObject *parent) : QObject(parent) {
}

void TranslationController::addData(Translations translations) {
    QQuickWidget *translationWidget = new QQuickWidget();

    data_.insert(translationWidget, translations);

    translationWidget->rootContext()->setContextProperty("trans",
                                                         QmlConvertation::prepareToQml(data_.value(translationWidget)));
    translationWidget->rootContext()->setContextProperty("controller", this);
    translationWidget->rootContext()->setContextProperty("self", translationWidget);
    translationWidget->rootContext()->setContextProperty("stateHolder", &StateHolder::Instance());
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
    data_.remove(widget);
    replaceWindows();
}

void TranslationController::createCard(QQuickWidget *widget, int index) {
    if (!(StateHolder::Instance()).isAuth()){
        SystemTray::Notify(QString("Not authorized"),
                           QString("You need to login if you want to create a new card"));
    } else if (data_[widget].size() > 0) {
        emit createNewCard(data_[widget][index]);
    } else {
        QSharedPointer<Translation> emptyData(new Translation());
        emit createNewCard(emptyData);
    }
}

void TranslationController::replaceWindows() {
    int absTopMargin = 50;
    int spacing = 20;

    int number = 0;
    for (auto widget : data_.keys()) {
        int yPos = absTopMargin + (widget->height() + spacing) * (number++);
        int xPos = QApplication::desktop()->screenGeometry().width() - widget->width() - 50;
        widget->move(xPos, yPos);
    }
}

//void TranslationController::createLater(quint32 id)
//{
//    //TODO: save to special buffer
//}
