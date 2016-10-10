#ifndef IDIFF_H
#define IDIFF_H

class DBCard;

class IDiff
{
public:
    IDiff();
    virtual void apply(DBCard * dbcard) = 0;
};

#endif // IDIFF_H
