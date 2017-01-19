//
// Created by alex on 18.01.17.
//

#include "QmlWidget.h"

void QmlWidget::closeEvent(QCloseEvent *event) {
    qDebug("Сlose widget");
    emit closed();
    QWidget::closeEvent(event);
}
