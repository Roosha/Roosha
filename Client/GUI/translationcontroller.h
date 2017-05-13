#ifndef TRANSLATIONCONTROLLER_H
#define TRANSLATIONCONTROLLER_H

#include <QObject>
#include <QtGlobal>
#include <QMap>
#include <QSharedPointer>
#include <QQuickWidget>
#include "Core/translation.h"

class TranslationController : public QObject {
 Q_OBJECT
 public:
    TranslationController(QObject *parent = nullptr);

    void addData(Translations translations);

    Q_INVOKABLE void closeWindow(QQuickWidget *widget);
    Q_INVOKABLE void createCard(QQuickWidget *widget, int index);
//    Q_INVOKABLE void createLater(quint32 id);

    static TranslationController *getInstance() {
        if(self == nullptr) {
            self = new TranslationController();
        }
        return self;
    }

 signals:
    void createNewCard(QSharedPointer<Translation> trans);

 private:
    void replaceWindows();

    QMap<QQuickWidget *, Translations> data_;

    static TranslationController *self;
};

static QObject *register_translation_controller(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    return TranslationController::getInstance();
}

#endif // TRANSLATIONCONTROLLER_H
