#ifndef DBCARD_H
#define DBCARD_H

#include "Core/card.h"
#include "Core/idiff.h"

class DBCard : public Card {
public:
    DBCard();

    void setSource(QString newSource);
    void setExamples(QVector<QString> newExamples);
    void setTarget(QVector<QString> newTarget);

    void saveToDB();
    void applyDiffs();
    void applyDiff(IDiff * diff);

private:
    quint32 lastSavedDiff;
    QVector<IDiff *> diffs;
};

#endif // DBCARD_H
