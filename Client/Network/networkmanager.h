#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "Core/translation.h"
#include <QObject>

class TestNetworkManager : public QObject {
    Q_OBJECT

public:
    TestNetworkManager(QObject * parent = Q_NULLPTR);
    ~TestNetworkManager();

    void requestTranslations(QString source);

signals:
    void newTranslation( TestTranslations trans);
};

#endif // NETWORKMANAGER_H
