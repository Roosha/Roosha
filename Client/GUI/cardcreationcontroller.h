#ifndef CARDCREATIONCONTROLLER_H
#define CARDCREATIONCONTROLLER_H
#include <QObject>
#include <QtGlobal>
#include <QMap>
#include <QSharedPointer>
#include <QQuickWidget>
#include "Core/translation.h"
#include "Core/world.h"
#include "Core/dbcard.h"

class CardCreationController : public QObject {
    Q_OBJECT
 public:
    CardCreationController(QObject * parent = nullptr);
    World& world;

    Q_INVOKABLE void closeWindow();
    Q_INVOKABLE void saveCard(QString src, QString tarStr, QString exStr);
signals:
    void showCards();

 public slots:
    void showNewEditWindow(QSharedPointer<Translation> trans);

 private:
    QQuickWidget*  widget;
    //TestTranslations data;
};

#endif // CARDCREATIONCONTROLLER_H
