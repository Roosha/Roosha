#ifndef CARD_H
#define CARD_H
#include <QtGlobal>
#include <QVector>
#include <QUuid>
#include <QObject>

class Card : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString source READ getSource)
    Q_PROPERTY(QString target READ getTargetString)
    Q_PROPERTY(QString examples READ getExampleString)
 public:
    Card();

    QString getSource() const;
    QStringList getTargets() const;
    QStringList getExamples() const;

    QString getTargetString() const;
    QString getExampleString() const;

 protected:
    QString source;
    QStringList targets;
    QStringList examples;
};

#endif // CARD_H
