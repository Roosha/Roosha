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
    void setExamples(QVector<QString> newExamples);
    void setTarget(QVector<QString> newTarget);
    void editExampleElem(QString newElem, qint32 pos);
    void editTargetElem(QString newElem, qint32 pos);
    void deleteExampleElem(qint32 pos);
    void deleteTargetElem(qint32 pos);
    void insertExampleElem(QString insertingElem, qint32 pos);
    void insertTargetElem(QString insertingElem, qint32 pos);

    void saveToDB();
    void applyChanges();

 private:
    friend class ChangeSource;
    friend class EditExampleElem;
    friend class InsertExampleElem;
    friend class DeleteExampleElem;
    friend class EditTargetElem;
    friend class InsertTargetElem;
    friend class DeleteTargetElem;
    quint32 lastSavedChange;
    QVector<QSharedPointer<IChange>> changes;
};

#endif // DBCARD_H
