#include "changes.h"
#include <QDebug>

ChangeSource::ChangeSource() { }

ChangeSource::ChangeSource(const QString & newSrc) : newSource(newSrc) {}

void ChangeSource::apply(DBCard * cardptr) {
    cardptr->source = newSource;
}
