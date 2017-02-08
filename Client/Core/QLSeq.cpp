//
// Created by alex on 20.01.17.
//

#include <climits>
#include <QQueue>
#include <exception>
#include <cassert>
#include <QDebug>
#include "QLSeq.h"

QLKey::QLKey() {}
QLKey::~QLKey() {}

QLKey::QLKey(QByteArray array) {
    data.reserve(array.size());
    for(char e : array) {
        data.push_back(e);
    }
}

char QLKey::at(int depth) const {
    if(data.size() > depth) {
        return data.at(depth);
    } else {
        return 0;
    }
}

QLKey::QLKey(const QLKey &other) {
    data = other.data;
}

QLKey::QLKey(std::initializer_list<char> list) : data(list) {}

bool QLKey::operator<(const QLKey &other) const {
    for(int i = 0; i < qMax(size(), other.size()); i++) {
        if(at(i) < other.at(i)) return true;
        else if(at(i) > other.at(i)) return false;
    }
    return false;
}

QByteArray QLKey::toByteArray() const {
    QByteArray bytes;
    bytes.reserve(data.size());
    for(char e : data) {
        bytes.push_back(e);
    }
    return bytes;
}

int QLKey::size() const {
    return data.size();
}

// ------------------------------------


QLSeq::QLSeq() : base(CHAR_MAX), boundary(CHAR_MAX / 32), gen(std::mt19937(rd())) {
    QLKey begin;
    QLKey end;
    begin.data.push_back(1);
    end.data.push_back(CHAR_MAX);

    tree.insert(begin, "b\n\0");
    tree.insert(end, "e\n\0");
}
QLSeq::~QLSeq() {}

void QLSeq::remove(QString element) {
    tree.remove(tree.key(element));
}

void QLSeq::remove(QLKey key) {
    tree.remove(key);
}

QLKey QLSeq::remove(int index) {
    QLKey key = (cbegin()+index).key();
    tree.remove(key);
    return key;
}

int QLSeq::size() const {
    return tree.size() - 2;
}

QLKey QLSeq::modify(int index, QString newValue) {
    QLKey key = (cbegin() + index).key();
    tree[key] = newValue;
    return key;
}

void QLSeq::modify(QLKey key, QString newValue) {
    tree[key] = newValue;
}

QMap<QLKey, QString>::const_iterator QLSeq::cbegin() const {
    return tree.cbegin()+1;
}

QMap<QLKey, QString>::const_iterator QLSeq::cend() const {
    return tree.cend()-1;
}

QLKey QLSeq::insertAfter(int index, QString newElement) {
    return insertBetween(newElement, index, index+1);
}

QLKey QLSeq::insertAfter(QLKey key, QString newElement) {
    auto left = tree.find(key);
    QLKey right = (left+1).key();
    return insertBetween(newElement, key, right);
}

QLKey QLSeq::insert(QLKey key, QString newElement) {
    tree.insert(key, newElement); // TODO: switch to MultiMap in order to resolve collision keys
    return key;
}

/**
 * Distance between p and q must be 1
 * @param newElement
 * @param p
 * @param q
 */
QLKey QLSeq::insertBetween(QString newElement, int p, int q) {
    assert(q-p == 1);

    QLKey left, right;

    auto it = cbegin();
    it += p;
    left = it.key();
    it++;
    right = it.key();

    return insertBetween(newElement, left, right);
}

QLKey QLSeq::firstKey() {
    return cbegin().key();
}

QLKey QLSeq::lastKey() {
    return (cend()-1).key();
}

QString QLSeq::at(QLKey key) {
    auto result = tree.find(key);
    if(result != tree.end()) {
        return result.value();
    } else {
        throw std::out_of_range("No key");
    }
}

QString QLSeq::at(int index) {
    if(index < 0 or index >= size()) throw std::out_of_range("No index");
    return (cbegin()+index).value();
}

QStringList QLSeq::toList() const {
    QList<QString> l = tree.values();
    l.pop_back();
    l.pop_front();
    return QStringList(l);
}

int QLSeq::strategy(int depth) {
    if(depth % 2 == 0) return 1;
    else return -1;
}

QLKey QLSeq::rightBorder(QLKey key, int depth) {
    QLKey border;
    border.data.resize(depth+1);
    for(int i = 0; i < depth; i++) {
        border.data[i] = key.at(i);
    }
    border.data[depth] = (char)base;
    return border;
}

QLKey QLSeq::leftBorder(QLKey key, int depth) {
    QLKey border;
    border.data.resize(depth+1);
    for(int i = 0; i < depth; i++) {
        border.data[i] = key.at(i);
    }
    border.data[depth] = 0;
    return border;
}

/**
 * Find free space in the LSequence Tree
 * @param p - left position
 * @param q - right position
 * @param depth - initial depth to search. 0, by default.
 * @return tuple <depth, rightPosition, leftPosition>, that means we find free space between rightPosition and leftPosition on depth level.
 */
std::tuple<int, QLKey, QLKey> QLSeq::findFreeSpace(QLKey p, QLKey q, int depth) {
    QQueue< std::tuple<int, QLKey, QLKey> > variants;
    variants.enqueue( std::make_tuple(depth, p, q) );
    for(;;) {
        QLKey left, right;
        int d;
        std::tie(d, left, right) = variants.dequeue();
        if(right.at(d) == left.at(d)) variants.enqueue(std::make_tuple(d+1, left, right));
        else if(right.at(d) - left.at(d) > 1) return std::make_tuple(d, left, right);
        else {
            if(strategy(d+1) == 1) {
                variants.enqueue( std::make_tuple(d+1, left, rightBorder(left, d+1)) );
                variants.enqueue( std::make_tuple(d+1, leftBorder(right, d+1), right) );
            } else {
                variants.enqueue( std::make_tuple(d+1, leftBorder(right, d+1), right) );
                variants.enqueue( std::make_tuple(d+1, left, rightBorder(left, d+1)) );
            }
        }
    }
}

QLKey QLSeq::allocate(QLKey p, QLKey q) {
    QLKey left, right;
    int d;
    std::tie(d, left, right) = findFreeSpace(p, q);

    std::uniform_int_distribution<int> dis(1, qMin(boundary, qAbs(left.at(d)-right.at(d) ) )-1 );

    QLKey newKey;
    newKey.data.resize(d+1);

    if(strategy(d) == 1) {
        for(int i = 0; i < d; i++) newKey.data[i] = left.at(i);
        newKey.data[d] = (char)(left.at(d) + dis(gen));
    } else {
        for(int i = 0; i < d; i++) newKey.data[i] = right.at(i);
        newKey.data[d] = (char)(right.at(d) - dis(gen));
    }

    return newKey;
}

QLKey QLSeq::insertBetween(QString newElement, QLKey p, QLKey q) {
    QLKey newKey = allocate(p, q);
    tree.insert(newKey, newElement);
    return newKey;
}