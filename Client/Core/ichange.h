#ifndef IDIFF_H
#define IDIFF_H

class DBCard;

class IChange {
 public:
    ~IChange() {}
    virtual void apply(DBCard * cardptr) = 0;
};

#endif // IDIFF_H
