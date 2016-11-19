#ifndef DBCARD_H
#define DBCARD_H
#include <QUuid>
#include <QSharedPointer>
#include "Core/card.h"
#include "Core/ichange.h"
#include "changes.h"
#include "ichange.h"
#include "world.h"

class DBCard : public Card {
 public:
    DBCard(QUuid newId);
    QUuid getId();

    void setSource(QString newSource);
    void setExamples(QStringList newExamples);
    void setTargets(QStringList newTarget);
    void editElem(const enum Field & fieldName, const QString & newElem, qint32 pos);
    void deleteElem(const enum Field & fieldName, qint32  pos);
    void insertElem(const enum Field & fieldName, const QString  &insertingElem, qint32 pos);

 private:
    friend class ChangeSource;
    friend class EditElem;
    friend class InsertElem;
    friend class DeleteElem;
    World& world_;
    QUuid id_;
};

#endif // DBCARD_H
