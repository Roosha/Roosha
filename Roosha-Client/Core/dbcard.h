#ifndef DBCARD_H
#define DBCARD_H
#include <QSharedPointer>
#include "Core/card.h"
#include "Core/ichange.h"
#include "changes.h"
#include "ichange.h"

class DBCard : public Card {
 public:
    DBCard();

    void setSource(QString newSource);
//    void setExamples(QVector<QString> newExamples);
//    void setTarget(QVector<QString> newTarget);

    void saveToDB();
    void applyChanges();

 private:
    friend class ChangeSource;
    quint32 lastSavedChange;
    QVector<QSharedPointer<IChange>> changes;
};

#endif // DBCARD_H
