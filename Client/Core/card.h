#ifndef CARD_H
#define CARD_H
#include <QtGlobal>
#include <QVector>
#include <QUuid>
class Card {
 public:
    Card();

    QString getSource();
    QStringList getTargets();
    QStringList getExamples();

 protected:
    QString source;
    QStringList targets;
    QStringList examples;
};

#endif // CARD_H
