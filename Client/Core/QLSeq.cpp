//
// Created by alex on 20.01.17.
//

#include <climits>
#include <QQueue>
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

QLSeq::QLSeq() : base(CHAR_MAX), boundary(CHAR_MAX / 4) {}
QLSeq::~QLSeq() {}

int QLSeq::strategy(int depth) {
    if(depth % 2 == 0) return 1;
    else return -1;
}

std::tuple<int, QLKey, QLKey> QLSeq::findFreeSpace(QLKey p, QLKey q, int depth = 0) {
    QQueue< std::tuple<int, QLKey, QLKey> > variants;
    variants.enqueue( std::make_tuple(depth, p, q) );
    for(;;) {
        QLKey right, left;
        int d;
        std::tie(d, right, left) = variants.dequeue();
        if(left.at(d) - right.at(d) > 1) return std::make_tuple(d, right, left);
        else {
            if(strategy(d+1) == 1) {
                variants.enqueue( std::make_tuple(d+1, right, rightBorder(right, d+1)) );
                variants.enqueue( std::make_tuple(d+1, leftBorder(left, d+1), left) );
            } else {
                variants.enqueue( std::make_tuple(d+1, leftBorder(left, d+1), left) );
                variants.enqueue( std::make_tuple(d+1, right, rightBorder(right, d+1)) );
            }
        }
    }
}

QLKey QLSeq::allocate(QLKey p, QLKey q) {

}

void QLSeq::insertBetween(QString newElement, QLKey p, QLKey q) {
    QLKey newKey = allocate(p, q);
}