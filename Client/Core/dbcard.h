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
    //@formatter:off
    Q_PROPERTY(QUuid id READ getId)
    //@formatter:on
 public:
    DBCard(QUuid newId);
    void setId(QUuid newId);
    void setSource(QString newSource);
    void setField(const enum Field &fieldName, QStringList newField);
    void editElem(const enum Field &fieldName, const QString &newElem, qint32 pos);
    void deleteElem(const enum Field &fieldName, qint32 pos);
    void insertElem(const enum Field &fieldName, const QString &insertingElem, qint32 pos);

 private:
    friend class ChangeSource;
    friend class EditElem;
    friend class InsertElem;
    friend class DeleteElem;
    World &world_;
};

#endif // DBCARD_H
