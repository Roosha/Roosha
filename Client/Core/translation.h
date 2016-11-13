#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

class Translation : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString source READ getSource)
    Q_PROPERTY(QString target READ getTargetString)
    Q_PROPERTY(QString examples READ getExampleString)

public:
    Translation();

    QString getSource() const;
    void setSource(const QString &value);

    QStringList getTarget() const;
    void setTarget(const QStringList &value);

    QString getTargetString() const;
    QString getExampleString() const;

    QStringList getExamples() const;
    void setExamples(const QStringList &value);

private:

    QString source;
    QStringList target;
    QStringList examples;
};

typedef QList<QSharedPointer<Translation> > Translations;

#endif // TRANSLATION_H
