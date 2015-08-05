#include "tattoo.h"

#include "characterclass.h"

#include <BeQtGlobal>

#include <QSettings>
#include <QString>
#include <QVariant>

Tattoo::Tattoo(const QString &confFileName)
{
    if (confFileName.isEmpty()) {
        mincreasedStat = CharacterClass::STR;
        mdecreasedStat = CharacterClass::STR;
        mincreasedValue = 0;
        mdecreasedValue = 0;
    } else {
        QSettings s(confFileName, QSettings::IniFormat);
        s.beginGroup("Tattoo");
        mincreasedStat = enum_cast<CharacterClass::Stat>(s.value("increased_stat"), CharacterClass::NoStat,
                                                         CharacterClass::MEN);
        mdecreasedStat = enum_cast<CharacterClass::Stat>(s.value("decreased_stat"), CharacterClass::NoStat,
                                                         CharacterClass::MEN);
        mincreasedValue = s.value("increased_value").toInt();
        mdecreasedValue = s.value("decreased_value").toInt();
        s.endGroup();
    }
}

Tattoo::Tattoo(const Tattoo &other)
{
    *this = other;
}

CharacterClass::Stat Tattoo::increasedStat() const
{
    return mincreasedStat;
}

CharacterClass::Stat Tattoo::decreasedStat() const
{
    return mdecreasedStat;
}

int Tattoo::increasedValue() const
{
    return mincreasedValue;
}

int Tattoo::decreasedValue() const
{
    return mdecreasedValue;
}

Tattoo &Tattoo::operator =(const Tattoo &other)
{
    mincreasedStat = other.mincreasedStat;
    mdecreasedStat = other.mdecreasedStat;
    mincreasedValue = other.mincreasedValue;
    mdecreasedValue = other.mdecreasedValue;
    return *this;
}

bool Tattoo::operator ==(const Tattoo &other) const
{
    return mincreasedStat == other.mincreasedStat && mdecreasedStat == other.mdecreasedStat
            && mincreasedValue == other.mincreasedValue && mdecreasedValue == other.mdecreasedValue;
}
