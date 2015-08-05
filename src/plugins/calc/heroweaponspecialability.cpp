#include "heroweaponspecialability.h"

#include <BeQtGlobal>

#include <QSettings>
#include <QString>
#include <QVariant>

HeroWeaponSpecialAbility::HeroWeaponSpecialAbility(QSettings *s)
{
    if (s) {
        mtype = enum_cast<Type>(s->value("type"), NoType, InfinityWing);
    } else {
        mtype = NoType;
    }
}

HeroWeaponSpecialAbility::HeroWeaponSpecialAbility(const HeroWeaponSpecialAbility &other)
{
    *this = other;
}

HeroWeaponSpecialAbility::Type HeroWeaponSpecialAbility::type() const
{
    return mtype;
}

HeroWeaponSpecialAbility &HeroWeaponSpecialAbility::operator =(const HeroWeaponSpecialAbility &other)
{
    mtype = other.mtype;
    return *this;
}

bool HeroWeaponSpecialAbility::operator ==(const HeroWeaponSpecialAbility &other) const
{
    return mtype == other.mtype;
}
