#ifndef CHARACTERCLASS_H
#define CHARACTERCLASS_H

#include <QMap>
#include <QString>

class CharacterClass
{
public:
    enum Stat
    {
        NoStat = 0,
        STR,
        DEX,
        CON,
        INT,
        WIT,
        MEN
    };
private:
    double mbaseHP1;
    double mbaseHP21;
    double mbaseHP41;
    QMap<Stat, int> mbaseStat;
    double mcpK;
    double mhpK11;
    double mhpK21;
    double mhpK121;
    double mhpK221;
    double mhpK141;
    double mhpK241;
    int mclassType1;
    int mclassType21;
    int mclassType41;
    int mmaxAllowedTattoos;
    QString mname;
    Stat mrestrictedTattoo;
public:
    explicit CharacterClass(const QString &confFileName = QString());
    CharacterClass(const CharacterClass &other);
public:
    static QString statToString(Stat stat);
public:
    double baseHP(int lvl) const;
    double baseHP1() const;
    double baseHP21() const;
    double baseHP41() const;
    int baseStat(Stat stat) const;
    double cpK() const;
    double hpK1(int lvl) const;
    double hpK2(int lvl) const;
    double hpK11() const;
    double hpK21() const;
    double hpK121() const;
    double hpK221() const;
    double hpK141() const;
    double hpK241() const;
    int classType(int lvl) const;
    int classType1() const;
    int classType21() const;
    int classType41() const;
    int maxAllowedTattoos() const;
    QString name() const;
    Stat restrictedTattoo() const;
public:
    CharacterClass &operator =(const CharacterClass &other);
    bool operator ==(const CharacterClass &other) const;
};

#endif // CHARACTERCLASS_H
