#ifndef CARD_H
#define CARD_H
#include <QtGlobal>
#include <QVector>
#include <QUuid>
#include <QObject>

class Card : public QObject {
 Q_OBJECT
    //@formatter:off
    Q_PROPERTY(QUuid id READ getId)
    Q_PROPERTY(QString source READ getSource)
    Q_PROPERTY(QString target READ getTargetString)
    Q_PROPERTY(QString examples READ getExampleString)
    //@formatter:on
 public:
    Card();

    QUuid getId() const;
    QString getSource() const;
    QStringList getTargets() const;
    QStringList getExamples() const;

    QString getTargetString() const;
    QString getExampleString() const;

 protected:

    QUuid id_;
    QString source_;
    QStringList targets_;
    QStringList examples_;
};

#endif // CARD_H
