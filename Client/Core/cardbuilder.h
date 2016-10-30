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
    CardBuilder& edit(const enum Field field, const QString &example, const qint32 &pos);
    CardBuilder& insert(const enum Field field, const QString &example, const qint32 &pos);
    CardBuilder& del(const enum Field field, const qint32 &pos);

    DBCard&& build();
//    CardBuilder& setExamples(QVector<QString> examples);
//    CardBuilder& setTargets(QVector<QString> targets);

 private:
    void check_flag();

    bool built;
    DBCard newCard;
};

#endif // CARDBUILDER_H
