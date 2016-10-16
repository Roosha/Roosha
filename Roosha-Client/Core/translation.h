#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QObject>
#include <QList>
#include <QSharedPointer>

class Translation : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString source READ getSource WRITE setSource)

public:
    Translation();
    Translation(QString d);

    QString getSource() const;
    void setSource(const QString &src);

private:
    QString data;
};

typedef QList<QSharedPointer<Translation> > Translations;

#endif // TRANSLATION_H
