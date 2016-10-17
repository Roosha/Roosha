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

#endif // CHANGES_H
