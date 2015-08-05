#include "specialability.h"

#include <BeQtGlobal>

#include <QSettings>
#include <QString>
#include <QVariant>

SpecialAbility::SpecialAbility(QSettings *s)
{
    if (s) {
        mtype = enum_cast<Type>(s->value("type"), NoType, MagicDamage);
        mvalue = s->value("value").toDouble();
    } else {
        mtype = NoType;
        mvalue = 0.0;
    }
}

SpecialAbility::SpecialAbility(const SpecialAbility &other)
{
    *this = other;
}

QString SpecialAbility::saTypeToString(Type type)
{
    switch (type) {
    case HPDrain:
        return "HP Drain";
    case Acumen:
        return "Acumen";
    case BackBlow:
        return "Back Blow";
    case Evasion:
        return "Evasion";
    case Focus:
        return "Focus";
    case Guidance:
        return "Guidance";
    case Haste:
        return "Haste";
    case Health:
        return "Health";
    case HPRegeneration:
        return "HP Regeneration";
    case ManaUp:
        return "Mana Up";
    case MPRegeneration:
        return "MP Regeneration";
    case QuickRecovery:
        return "Quick Recovery";
    case CheapShot:
        return "Cheap Shot";
    case Light:
        return "Light";
    case LongBlow:
        return "Long Blow";
    case Miser:
        return "Miser";
    case WideBlow:
        return "Wide Blow";
    case Anger:
        return "Anger";
    case Conversion:
        return "Conversion";
    case Empower:
        return "Empower";
    case MagicPower:
        return "Magic Power";
    case CriticalAnger:
        return "Critical Anger";
    case CriticalBleed:
        return "Critical Bleed";
    case CriticalDamage:
        return "Critical Damage";
    case CriticalDrain:
        return "Critical Drain";
    case CriticalPoison:
        return "Critical Poison";
    case CriticalSlow:
        return "Critical Slow";
    case CriticalStun:
        return "Critical Stun";
    case CrtDamage:
        return "Crt. Damage";
    case CrtStun:
        return "Crt. Stun";
    case RskEvasion:
        return "Rsk. Evasion";
    case RskFocus:
        return "Rsk. Focus";
    case RskHaste:
        return "Rsk. Haste";
    case BodilyBlessing:
        return "Bodily Blessing";
    case BlessTheBody:
        return "Bless the Body";
    case MagicChaos:
        return "Magic Chaos";
    case MagicFocus:
        return "Magic Focus";
    case MagicMentalShield:
        return "Magic Mental Shield";
    case MagicRegeneration:
        return "Magic Regeneration";
    case MentalShield:
        return "Mental Shield";
    case MagicHold:
        return "Magic Hold";
    case MagicParalyze:
        return "Magic Paralyze";
    case MagicPoison:
        return "Magic Poison";
    case MagicSilence:
        return "Magic Silence";
    case MagicWeakness:
        return "Magic Weakness";
    case MagicDamage:
        return "Magic Damage";
    default:
        return "No";
    }
}

SpecialAbility::Type SpecialAbility::type() const
{
    return mtype;
}

double SpecialAbility::value() const
{
    return mvalue;
}

SpecialAbility &SpecialAbility::operator =(const SpecialAbility &other)
{
    mtype = other.mtype;
    mvalue = other.mvalue;
    return *this;
}

bool SpecialAbility::operator ==(const SpecialAbility &other) const
{
    return mtype == other.mtype && mvalue == other.mvalue;
}
