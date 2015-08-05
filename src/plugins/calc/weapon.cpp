#include "weapon.h"

#include "dualswordspecialability.h"
#include "grade.h"
#include "heroweaponspecialability.h"
#include "specialability.h"

#include <BeQtGlobal>

#include <QDebug>
#include <QList>
#include <QSettings>
#include <QString>
#include <QVariant>

Weapon::Weapon(const QString &confFileName)
{
    if (confFileName.isEmpty()) {
        mgrade = Grade::NoGrade;
        mmatk = 0;
        mpatk = 0;
        mtype = NoType;
        mtwoHanded = false;
    } else {
        QSettings s(confFileName, QSettings::IniFormat);
        s.beginGroup("Weapon");
        mgrade = Grade::gradeFromString(s.value("grade").toString());
        mmatk = s.value("matk").toInt();
        mname = s.value("name").toString();
        mpatk = s.value("patk").toInt();
        mtype = enum_cast<Type>(s.value("type"), NoType, Sword);
        mtwoHanded = s.value("two_handed").toBool();
        if (mgrade >= Grade::CGrade) {
            if (DualSword == mtype) {
                s.beginGroup("dual_sword_sa");
                mdualSA = DualSwordSpecialAbility(&s);
                s.endGroup();
            } else {
                foreach (int i, bRangeD(1, 3)) {
                    s.beginGroup("sa_" + QString::number(i));
                    SpecialAbility sa(&s);
                    if (sa.type() == SpecialAbility::NoType) {
                        s.endGroup();
                        break;
                    }
                    msa << sa;
                    s.endGroup();
                }
            }
            if (Grade::SGrade == mgrade) {
                s.beginGroup("hero_sa");
                mheroSA = HeroWeaponSpecialAbility(&s);
                s.endGroup();
            }
        }
        s.endGroup();
    }
}

Weapon::Weapon(const Weapon &other)
{
    *this = other;
}

QString Weapon::weaponTypeToString(Type type)
{
    switch (type) {
    case Blunt:
        return "Blunt";
    case Bow:
        return "Bow";
    case Dagger:
        return "Dagger";
    case DualSword:
        return "Dual Sword";
    case Fist:
        return "Fist";
    case Polearm:
        return "Polearm";
    case Sword:
        return "Sword";
    case NoType:
    default:
        return "";
    }
}

const DualSwordSpecialAbility &Weapon::dualSwordSA() const
{
    return mdualSA;
}

Grade::GradeType Weapon::grade() const
{
    return mgrade;
}

const HeroWeaponSpecialAbility &Weapon::heroWeaponSA() const
{
    return mheroSA;
}

int Weapon::matk() const
{
    return mmatk;
}

QString Weapon::name() const
{
    return mname;
}

int Weapon::patk() const
{
    return mpatk;
}

const QList<SpecialAbility> &Weapon::sa() const
{
    return msa;
}

Weapon::Type Weapon::type() const
{
    return mtype;
}

bool Weapon::twoHanded() const
{
    return mtwoHanded;
}

Weapon &Weapon::operator =(const Weapon &other)
{
    mdualSA = other.mdualSA;
    mgrade = other.mgrade;
    mheroSA = other.mheroSA;
    mmatk = other.mmatk;
    mname = other.mname;
    mpatk = other.mpatk;
    msa = other.msa;
    mtype = other.mtype;
    mtwoHanded = other.mtwoHanded;
    return *this;
}

bool Weapon::operator ==(const Weapon &other) const
{
    return mname == other.mname;
}
