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
    TranslationController(QObject * parent = nullptr);

    void addData(TestTranslations translations);

    Q_INVOKABLE void closeWindow(quint32 id);
    Q_INVOKABLE void createCard(quint32 id);
//    Q_INVOKABLE void createLater(quint32 id);

signals:
    void createNewCard(QSharedPointer<Translation> trans);

 private:
    quint32 lastId;

    QMap<quint32, QQuickWidget* > widgets;
    QMap<quint32, TestTranslations> data;
};

#endif // TRANSLATIONCONTROLLER_H
