#ifndef HEROWEAPONSPECIALABILITY_H
#define HEROWEAPONSPECIALABILITY_H

class QSettings;

class HeroWeaponSpecialAbility
{
public:
    enum Type
    {
        NoType = 0,
        InfinityAxe,
        InfinityBlade,
        InfinityBow,
        InfinityCleaver,
        InfinityCrusher,
        InfinityFang,
        InfinityRod,
        InfinityScepter,
        InfinitySpear,
        InfinityStinger,
        InfinityWing
    };
private:
    Type mtype;
public:
    explicit HeroWeaponSpecialAbility(QSettings *s = 0);
    HeroWeaponSpecialAbility(const HeroWeaponSpecialAbility &other);
public:
    Type type() const;
public:
    HeroWeaponSpecialAbility &operator =(const HeroWeaponSpecialAbility &other);
    bool operator ==(const HeroWeaponSpecialAbility &other) const;
};

#endif // HEROWEAPONSPECIALABILITY_H
