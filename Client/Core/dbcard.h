#ifndef DBCARD_H
#define DBCARD_H
#include <QUuid>
#include <QSharedPointer>
#include "Core/card.h"
#include "Core/ichange.h"
#include "changes.h"
#include "ichange.h"

class DBCard : public Card {
 public:
    DBCard();
    DBCard(QUuid id);
    QUuid getId();

 private:
    friend class ChangeSource;
    friend class EditElem;
    friend class InsertElem;
    friend class DeleteElem;

    QUuid id;
};

#endif // DBCARD_H
