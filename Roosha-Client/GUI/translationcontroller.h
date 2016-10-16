#ifndef TRANSLATIONCONTROLLER_H
#define TRANSLATIONCONTROLLER_H

#include <QObject>
#include "Core/translation.h"

class TranslationController : public QObject {
    Q_OBJECT
public:
    TranslationController(QObject * parent = nullptr);
    void setData(Translations data);
    void show();

private:
    Translations data;

public slots:
    void clearData();
};

#endif // TRANSLATIONCONTROLLER_H
