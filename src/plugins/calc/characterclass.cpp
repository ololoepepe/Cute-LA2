#include "characterclass.h"

#include <BeQtGlobal>

#include <QDebug>
#include <QMap>
#include <QSettings>
#include <QString>
#include <QVariant>

CharacterClass::CharacterClass(const QString &confFileName)
{
    if (confFileName.isEmpty()) {
        mbaseHP1 = 0.0;
        mbaseHP21 = 0.0;
        mbaseHP41 = 0.0;
        mcpK = 0.0;
        mhpK11 = 0.0;
        mhpK21 = 0.0;
        mhpK121 = 0.0;
        mhpK221 = 0.0;
        mhpK141 = 0.0;
        mhpK241 = 0.0;
        mclassType1 = 0;
        mclassType21 = 0;
        mclassType41 = 0;
        mrestrictedTattoo = NoStat;
        mmaxAllowedTattoos = 0;
    } else {
        QSettings s(confFileName, QSettings::IniFormat);
        s.beginGroup("Class");
        mbaseHP1 = s.value("hp_base_1").toDouble();
        mbaseHP21 = s.value("hp_base_21", mbaseHP1).toDouble();
        mbaseHP41 = s.value("hp_base_41", mbaseHP21).toDouble();
        mcpK = s.value("cp_k").toDouble();
        mhpK11 = s.value("hp_k_1_1").toDouble();
        mhpK21 = s.value("hp_k_2_1").toDouble();
        mhpK121 = s.value("hp_k_1_21", mhpK11).toDouble();
        mhpK221 = s.value("hp_k_2_21", mhpK21).toDouble();
        mhpK141 = s.value("hp_k_1_41", mhpK121).toDouble();
        mhpK241 = s.value("hp_k_2_41", mhpK221).toDouble();
        mclassType1 = s.value("class_type_1").toInt();
        mclassType21 = s.value("class_type_21", mclassType1).toInt();
        mclassType41 = s.value("class_type_41", mclassType21).toInt();
        mname = s.value("name").toString();
        mrestrictedTattoo = enum_cast<Stat>(s.value("restricted_tattoo", NoStat), NoStat, MEN);
        mmaxAllowedTattoos = s.value("max_allowed_tattoos").toInt();
        int statType = s.value("stat_type").toInt();
        s.endGroup();
        QSettings ss(":/calc/data/base-stat.conf", QSettings::IniFormat);
        ss.beginGroup("Type" + QString::number(statType));
        mbaseStat.insert(STR, ss.value("STR").toInt());
        mbaseStat.insert(DEX, ss.value("DEX").toInt());
        mbaseStat.insert(CON, ss.value("CON").toInt());
        mbaseStat.insert(INT, ss.value("INT").toInt());
        mbaseStat.insert(WIT, ss.value("WIT").toInt());
        mbaseStat.insert(MEN, ss.value("MEN").toInt());
        ss.endGroup();
    }
}

CharacterClass::CharacterClass(const CharacterClass &other)
{
    *this = other;
}

QString CharacterClass::statToString(Stat stat)
{
    switch (stat) {
    case STR:
        return "STR";
    case DEX:
        return "DEX";
    case CON:
        return "CON";
    case INT:
        return "INT";
    case WIT:
        return "WIT";
    case MEN:
        return "MEN";
    default:
        return "";
    }
}

double CharacterClass::baseHP(int lvl) const
{
    if (lvl <= 0 || lvl > 80)
        return 0.0;
    if (lvl <= 20)
        return mbaseHP1;
    else if (lvl <= 40)
        return mbaseHP21;
    else
        return mbaseHP41;
}

double CharacterClass::baseHP1() const
{
    return mbaseHP1;
}

double CharacterClass::baseHP21() const
{
    return mbaseHP21;
}

double CharacterClass::baseHP41() const
{
    return mbaseHP41;
}

int CharacterClass::baseStat(Stat stat) const
{
    return mbaseStat.value(stat);
}

double CharacterClass::cpK() const
{
    return mcpK;
}

double CharacterClass::hpK1(int lvl) const
{
    if (lvl <= 0 || lvl > 80)
        return 0.0;
    if (lvl <= 20)
        return mhpK11;
    else if (lvl <= 40)
        return mhpK121;
    else
        return mhpK141;
}

double CharacterClass::hpK2(int lvl) const
{
    if (lvl <= 0 || lvl > 80)
        return 0.0;
    if (lvl <= 20)
        return mhpK21;
    else if (lvl <= 40)
        return mhpK221;
    else
        return mhpK241;
}

double CharacterClass::hpK11() const
{
    return mhpK11;
}

double CharacterClass::hpK21() const
{
    return mhpK21;
}

double CharacterClass::hpK121() const
{
    return mhpK121;
}

double CharacterClass::hpK221() const
{
    return mhpK221;
}

double CharacterClass::hpK141() const
{
    return mhpK141;
}

double CharacterClass::hpK241() const
{
    return mhpK241;
}

int CharacterClass::classType(int lvl) const
{
    if (lvl <= 0 || lvl > 80)
        return 0.0;
    if (lvl <= 20)
        return mclassType1;
    else if (lvl <= 40)
        return mclassType21;
    else
        return mclassType41;
}

int CharacterClass::classType1() const
{
    return mclassType1;
}

int CharacterClass::classType21() const
{
    return mclassType21;
}

int CharacterClass::classType41() const
{
    return mclassType41;
}

int CharacterClass::maxAllowedTattoos() const
{
    return mmaxAllowedTattoos;
}

QString CharacterClass::name() const
{
    return mname;
}

CharacterClass::Stat CharacterClass::restrictedTattoo() const
{
    return mrestrictedTattoo;
}

CharacterClass &CharacterClass::operator =(const CharacterClass &other)
{
    mname = other.mname;
    mbaseHP1 = other.mbaseHP1;
    mbaseHP21 = other.mbaseHP21;
    mbaseHP41 = other.mbaseHP41;
    mbaseStat = other.mbaseStat;
    mcpK = other.mcpK;
    mhpK11 = other.mhpK11;
    mhpK21 = other.mhpK21;
    mhpK121 = other.mhpK121;
    mhpK221 = other.mhpK221;
    mhpK141 = other.mhpK141;
    mhpK241 = other.mhpK241;
    mclassType1 = other.mclassType1;
    mclassType21 = other.mclassType21;
    mclassType41 = other.mclassType41;
    mrestrictedTattoo = other.mrestrictedTattoo;
    mmaxAllowedTattoos = other.mmaxAllowedTattoos;
    return *this;
}

bool CharacterClass::operator ==(const CharacterClass &other) const
{
    return mname == other.mname;
}
