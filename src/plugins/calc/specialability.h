#ifndef SPECIALABILITY_H
#define SPECIALABILITY_H

class QSettings;
class QString;

class SpecialAbility
{
public:
    enum Type
    {
        NoType = 0,
        //Permanent active
        HPDrain,
        //Permanent boost stat
        Acumen,
        BackBlow,
        Evasion,
        Focus,
        Guidance,
        Haste,
        Health,
        HPRegeneration,
        ManaUp,
        MPRegeneration,
        QuickRecovery,
        //Permanent boost other
        CheapShot,
        Light,
        LongBlow,
        Miser,
        WideBlow,
        //Permanent convert
        Anger,
        Conversion,
        Empower,
        MagicPower,
        //Critical
        CriticalAnger,
        CriticalBleed,
        CriticalDamage,
        CriticalDrain,
        CriticalPoison,
        CriticalSlow,
        CriticalStun,
        CrtDamage,
        CrtStun,
        //Rsk.
        RskEvasion,
        RskFocus,
        RskHaste,
        //Buff
        BodilyBlessing,
        BlessTheBody,
        MagicChaos,
        MagicFocus,
        MagicMentalShield,
        MagicRegeneration,
        MentalShield,
        //Debuff
        MagicHold,
        MagicParalyze,
        MagicPoison,
        MagicSilence,
        MagicWeakness,
        //Debuff boost
        MagicDamage
    };
private:
    Type mtype;
    double mvalue;
public:
    explicit SpecialAbility(QSettings *s = 0);
    SpecialAbility(const SpecialAbility &other);
public:
    static QString saTypeToString(Type type);
public:
    Type type() const;
    double value() const;
public:
    SpecialAbility &operator =(const SpecialAbility &other);
    bool operator ==(const SpecialAbility &other) const;
};

#endif // SPECIALABILITY_H
