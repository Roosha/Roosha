#ifndef CARD_H
#define CARD_H
#include <QtGlobal>
#include <QVector>

class Card {
 public:
    Card();

    QString getSource();
//    QVector<QString> getTarget();
//    QVector<QString> getExamples();

 protected:
   // quint32 id;
    QString source;
//    QVector<QString> target;
//    QVector<QString> examples;
};

#endif // CARD_H
