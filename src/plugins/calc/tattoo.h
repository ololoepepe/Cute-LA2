#ifndef TATTOO_H
#define TATTOO_H

#include "characterclass.h"

#include <QString>

class Tattoo
{
private:
    CharacterClass::Stat mincreasedStat;
    CharacterClass::Stat mdecreasedStat;
    int mincreasedValue;
    int mdecreasedValue;
public:
    explicit Tattoo(const QString &confFileName = QString());
    Tattoo(const Tattoo &other);
public:
    CharacterClass::Stat increasedStat() const;
    CharacterClass::Stat decreasedStat() const;
    int increasedValue() const;
    int decreasedValue() const;
public:
    Tattoo &operator =(const Tattoo &other);
    bool operator ==(const Tattoo &other) const;
};

#endif // TATTOO_H
