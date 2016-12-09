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
#include <QUuid>

class CardEditionController : public QObject {
    Q_OBJECT
public:
    CardEditionController(QObject * parent = nullptr);
    World& world;

    Q_INVOKABLE void closeWindow();
    Q_INVOKABLE void saveCard(QString src, QString tarStr, QString exStr, QUuid id);
signals:
    void showCards();

public slots:
    void showNewCardEditWindow(QSharedPointer<Translation> trans);
    void showCardEditWindow(QUuid id);

private:
    QQuickWidget*  widget;
    QSharedPointer<Translation> data;
};

#endif // CARDCREATIONCONTROLLER_H
