#ifndef CHANGES_H
#define CHANGES_H
#include "ichange.h"

#include <QSharedPointer>

#include "dbcard.h"

class ChangeSource : public IChange {
 public:

    ChangeSource();
    ChangeSource(const QString & newSrc);

    void apply(DBCard * cardptr);

 private:
    QString newSource;
};

class EditElem : public IChange {
 public:

    EditElem();
    EditElem(const enum Field & field, const QString & newEl, const qint32 & p);

    void apply(DBCard * cardptr);

 private:
    enum Field fieldName;
    QString newElem;
    qint32 pos;
};

class InsertElem : public IChange {
 public:

    InsertElem();
    InsertElem(const enum Field & field, const QString & insertingEl, const qint32 & p);

    void apply(DBCard * cardptr);

 private:
    enum Field fieldName;
    QString insertingElem;
    qint32 pos;
};

class DeleteElem : public IChange {
 public:

    DeleteElem();
    DeleteElem(const enum Field & field, const qint32 & p);

    void apply(DBCard * cardptr);

 private:
    enum Field fieldName;
    qint32 pos;
};

#endif // CHANGES_H
