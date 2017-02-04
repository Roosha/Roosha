//
// Created by alex on 20.01.17.
//

#ifndef ROOSHA_CLIENT_QLSEQ_H
#define ROOSHA_CLIENT_QLSEQ_H


#include <QtCore/QObject>
#include <QtCore/QVector>
#include <QByteArray>
#include <QtMath>
#include <QtCore/QMap>
#include <tuple>
#include <random>
#include <QList>

class QLSeq;
class QLKey;

class QLKey {
    friend class QLSeq;
    Q_GADGET
public:
    QLKey();
    QLKey(QByteArray array);
    QLKey(std::initializer_list<char> list);
    QLKey(const QLKey& other);
    ~QLKey();

    char at(int depth) const;
    bool operator<(const QLKey & other) const;
    QByteArray toByteArray();
    int size() const;

//private:
    QVector<char> data;
};

// --------------------------------------


class QLSeq {
    Q_GADGET
public:
    QLSeq();
    ~QLSeq();

    QStringList toList();
    QMap<QLKey, QString>::const_iterator cbegin() const;
    QMap<QLKey, QString>::const_iterator cend() const;
    int size() const;

    QLKey lastKey();
    QLKey firstKey();

    QString at(QLKey key);
    QString at(int index);

    void remove(QString element);
    void remove(QLKey key);

    void insertAfter(QLKey key, QString newElement);
    void insertAfter(int index, QString newElement);

    void insert(QLKey key, QString newElement);

private:
    void insertBetween(QString newElement, QLKey p, QLKey q);
    void insertBetween(QString newElement, int p, int q); // между p-ым и q-тым по счету

    std::tuple<int, QLKey, QLKey> findFreeSpace(QLKey p, QLKey q, int depth = 0);
    QLKey allocate(QLKey p, QLKey q);
    int strategy(int depth);

    QLKey rightBorder(QLKey key, int depth);
    QLKey leftBorder(QLKey key, int depth);

    QMap<QLKey, QString> tree;
    int base;
    int boundary;

    std::random_device rd;
    std::mt19937 gen;
};



#endif //ROOSHA_CLIENT_QLSEQ_H
