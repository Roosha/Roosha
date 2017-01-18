//
// Created by alex on 18.01.17.
//

#ifndef ROOSHACLIENT_QMLWIDGET_H
#define ROOSHACLIENT_QMLWIDGET_H

#include <QQuickWidget>
#include <QCloseEvent>

class QmlWidget : public QQuickWidget {
    Q_OBJECT
signals:
    void closed();

protected:
    void closeEvent(QCloseEvent *event) override;
};


#endif //ROOSHACLIENT_QMLWIDGET_H
