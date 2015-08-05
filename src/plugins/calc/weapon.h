#ifndef WEAPON_H
#define WEAPON_H

#include "dualswordspecialability.h"
#include "grade.h"
#include "heroweaponspecialability.h"
#include "specialability.h"

#include <QList>
#include <QString>

class Weapon
{
public:
    enum Type
    {
        NoType = 0,
        Blunt,
        Bow,
        Dagger,
        DualSword,
        Fist,
        Polearm,
        Sword
    };
private:
    DualSwordSpecialAbility mdualSA;
    Grade::GradeType mgrade;
    HeroWeaponSpecialAbility mheroSA;
    int mmatk;
    QString mname;
    int mpatk;
    QList<SpecialAbility> msa;
    Type mtype;
    bool mtwoHanded;
public:
    explicit Weapon(const QString &confFileName = QString());
    Weapon(const Weapon &other);
public:
    static QString weaponTypeToString(Type type);
public:
    const DualSwordSpecialAbility &dualSwordSA() const;
    Grade::GradeType grade() const;
    const HeroWeaponSpecialAbility &heroWeaponSA() const;
    int matk() const;
    QString name() const;
    int patk() const;
    const QList<SpecialAbility> &sa() const;
    Type type() const;
    bool twoHanded() const;
public:
    Weapon &operator =(const Weapon &other);
    bool operator ==(const Weapon &other) const;
};

#endif // WEAPON_H
