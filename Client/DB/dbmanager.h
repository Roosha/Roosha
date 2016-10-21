#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "Core/dbcard.h"

class DBManager
{
public:
    DBManager();

    QVector<DBCard> getCards();
    void saveCard(DBCard card);
};

#endif // DBMANAGER_H
