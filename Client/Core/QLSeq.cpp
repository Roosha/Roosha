//
// Created by alex on 20.01.17.
//

#include <climits>
#include <QQueue>
#include <exception>
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

QByteArray QLKey::toByteArray() {
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


QLSeq::QLSeq() : base(CHAR_MAX), boundary(CHAR_MAX / 4), gen(std::mt19937(rd())) {
    QLKey begin;
    QLKey end;
    begin.data.push_back(0);
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

int QLSeq::size() const {
    return tree.size();
}

QMap<QLKey, QString>::const_iterator QLSeq::cbegin() const {
    return tree.cbegin()+1;
}

QMap<QLKey, QString>::const_iterator QLSeq::cend() const {
    return tree.cend()-1;
}

/**
 * Distance between p and q must be 1
 * @param newElement
 * @param p
 * @param q
 */
void QLSeq::insertBetween(QString newElement, int p, int q) {
    if(q-p != 1) {
        throw std::invalid_argument("Distance between p and q must be 1");
    }

    QLKey left, right;

    auto it = cbegin();
    it += p;
    left = it.key();
    it++;
    right = it.key();

    insertBetween(newElement, left, right);
}

QStringList QLSeq::toList() {
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
    newKey.data.resize(d);

    if(strategy(d) == 1) {
        for(int i = 0; i < d; i++) newKey.data[i] = left.at(i);
        newKey.data[d] = (char)(left.at(d) + dis(gen));
    } else {
        for(int i = 0; i < d; i++) newKey.data[i] = right.at(i);
        newKey.data[d] = (char)(right.at(d) - dis(gen));
    }

    return newKey;
}

void QLSeq::insertBetween(QString newElement, QLKey p, QLKey q) {
    QLKey newKey = allocate(p, q);
    tree.insert(newKey, newElement);
}