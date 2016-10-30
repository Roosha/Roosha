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
    void editElem(const enum Field & fieldName, const QString & newElem, qint32 pos);
    void deleteElem(const enum Field & fieldName, qint32  pos);
    void insertElem(const enum Field & fieldName, const QString  &insertingElem, qint32 pos);

    void saveToDB();
    void applyChanges();

 private:
    friend class ChangeSource;
    friend class EditElem;
    friend class InsertElem;
    friend class DeleteElem;
    quint32 lastSavedChange;
    QVector<QSharedPointer<IChange>> changes;
};

#endif // DBCARD_H
