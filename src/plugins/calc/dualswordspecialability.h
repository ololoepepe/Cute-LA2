#ifndef DUALSWORDSPECIALABILITY_H
#define DUALSWORDSPECIALABILITY_H

class QSettings;

class DualSwordSpecialAbility
{
public:
    enum Type
    {
        NoType = 0,
        Focus,
        Guidance,
        Haste,
        Health,
        CriticalDamage,
        SDuals
    };
private:
    Type mtype;
    double mvalue;
public:
    explicit DualSwordSpecialAbility(QSettings *s = 0);
    DualSwordSpecialAbility(const DualSwordSpecialAbility &other);
public:
    Type type() const;
    double value() const;
public:
    DualSwordSpecialAbility &operator =(const DualSwordSpecialAbility &other);
    bool operator ==(const DualSwordSpecialAbility &other) const;
};

#endif // DUALSWORDSPECIALABILITY_H
