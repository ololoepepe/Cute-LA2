#include "dualswordspecialability.h"

#include <BeQtGlobal>

#include <QSettings>
#include <QString>
#include <QVariant>

DualSwordSpecialAbility::DualSwordSpecialAbility(QSettings *s)
{
    if (s) {
        mtype = enum_cast<Type>(s->value("type"), NoType, SDuals);
        mvalue = s->value("value").toDouble();
    } else {
        mtype = NoType;
        mvalue = 0.0;
    }
}

DualSwordSpecialAbility::DualSwordSpecialAbility(const DualSwordSpecialAbility &other)
{
    *this = other;
}

DualSwordSpecialAbility::Type DualSwordSpecialAbility::type() const
{
    return mtype;
}

double DualSwordSpecialAbility::value() const
{
    return mvalue;
}

DualSwordSpecialAbility &DualSwordSpecialAbility::operator =(const DualSwordSpecialAbility &other)
{
    mtype = other.mtype;
    mvalue = other.mvalue;
    return *this;
}

bool DualSwordSpecialAbility::operator ==(const DualSwordSpecialAbility &other) const
{
    return mtype == other.mtype && mvalue == other.mvalue;
}
