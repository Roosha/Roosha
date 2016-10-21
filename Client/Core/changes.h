#ifndef CHANGES_H
#define CHANGES_H
#include "ichange.h"
#include "dbcard.h"
#include <QSharedPointer>


class ChangeSource : public IChange {
 public:

    ChangeSource();
    ChangeSource(const QString & newSrc);

    void apply(DBCard * cardptr);

 private:
    QString newSource;
};

class EditExampleElem : public IChange {
 public:

    EditExampleElem();
    EditExampleElem(const QString & newEl, const qint32 & p);

    void apply(DBCard * cardptr);

 private:
    QString newElem;
    qint32 pos;
};

class InsertExampleElem : public IChange {
 public:

    InsertExampleElem();
    InsertExampleElem(const QString & insertingEl, const qint32 & p);

    void apply(DBCard * cardptr);

 private:
    QString insertingElem;
    qint32 pos;
};

class DeleteExampleElem : public IChange {
 public:

    DeleteExampleElem();
    DeleteExampleElem(const qint32 & p);

    void apply(DBCard * cardptr);

 private:
    qint32 pos;
};

class EditTargetElem : public IChange {
 public:

    EditTargetElem();
    EditTargetElem(const QString & newEl, const qint32 & p);

    void apply(DBCard * cardptr);

 private:
    QString newElem;
    qint32 pos;
};

class InsertTargetElem : public IChange {
 public:

    InsertTargetElem();
    InsertTargetElem(const QString & insertingEl, const qint32 & p);

    void apply(DBCard * cardptr);

 private:
    QString insertingElem;
    qint32 pos;
};

class DeleteTargetElem : public IChange {
 public:

    DeleteTargetElem();
    DeleteTargetElem(const qint32 & p);

    void apply(DBCard * cardptr);

 private:
    qint32 pos;
};

#endif // CHANGES_H
