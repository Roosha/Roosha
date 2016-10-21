#ifndef CARDBUILDER_H
#define CARDBUILDER_H
#include "dbcard.h"

class CardBuilder{
 public:
    CardBuilder();
    CardBuilder(const CardBuilder &) = delete;
    CardBuilder &operator=(const CardBuilder &) = delete;
    ~CardBuilder() {}

    CardBuilder& setSource(const QString &sourse);
    CardBuilder& editExample(const QString &example, const qint32 &pos);
    CardBuilder& insertExample(const QString &example, const qint32 &pos);
    CardBuilder& deleteExample(const qint32 &pos);
    CardBuilder& editTarget(const QString &target, const qint32 &pos);
    CardBuilder& insertTarget(const QString &target, const qint32 &pos);
    CardBuilder& deleteTarget(const qint32 &pos);

    DBCard&& build();
//    CardBuilder& setExamples(QVector<QString> examples);
//    CardBuilder& setTargets(QVector<QString> targets);

 private:
    void check_flag();

    bool built;
    DBCard newCard;
};

#endif // CARDBUILDER_H
