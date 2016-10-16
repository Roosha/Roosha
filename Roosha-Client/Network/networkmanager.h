#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include "Core/translation.h"
#include <QObject>

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    NetworkManager(QObject * parent = Q_NULLPTR);
    ~NetworkManager();

    void requestTranslations(QString source);

signals:
    void newTranslation( Translations trans);
};

#endif // NETWORKMANAGER_H
