#ifndef CALCWIDGET_H
#define CALCWIDGET_H

class BTranslation;

class QCheckBox;
class QComboBox;
class QGridLayout;
class QHBoxLayout;
class QLabel;
class QVBoxLayout;

#include "characterclass.h"
#include "tattoo.h"
#include "weapon.h"

#include <QList>
#include <QMap>
#include <QWidget>

class CalcWidget : public QWidget
{
    Q_OBJECT
private:
    enum EquipmentType
    {
        WeaponEquipment = 1,
        ShieldEquipment,
        HelmetEquipment,
        UpperBodyEquipment,
        LowerBodyEquipment,
        GlovesEquipment,
        BootsEquipment,
        NecklaceEquipment,
        Earring1Equipment,
        Earring2Equipment,
        Ring1Equipment,
        Ring2Equipment
    };
    enum Option
    {
        CombatOption,
        EquipmentOption,
        PassiveSkillOption,
        BuffOption,
        ToggleOption,
        DebuffOption
    };
private:
    struct EquipmentRow
    {
        QLabel *lbl;
        QComboBox *cmboxEnchant;
        QComboBox *cmboxGrade;
        QComboBox *cmbox;
    public:
        explicit EquipmentRow();
        explicit EquipmentRow(CalcWidget *self, QGridLayout *glt, int row, const BTranslation &t);
    };
private:
    QMap<int, CharacterClass> mclasses;
    QMap< int, QMap<int, double> > mbaseMP;
    QMap< CharacterClass::Stat, QMap<int, double> > mstatModifiers;
    QMap< Grade::GradeType, QMap< Weapon::Type, QMap<bool, int> > > mweaponPatkEnchantBonus;
    QMap<Grade::GradeType, int> mweaponMatkEnchantBonus;
    QMap<int, Tattoo> mtattoos;
    QMap<int, Weapon> mweapons;
    QMap<EquipmentType, EquipmentRow> mequipment;
    QComboBox *mcmboxClass;
    QComboBox *mcmboxLevel;
    QComboBox *mcmboxArmorSet;
    QComboBox *mcmboxJewelrySet;
    QComboBox *mcmboxSA;
    QList<QCheckBox *> mcboxTattoo;
    QList<QComboBox *> mcmboxTattoo;
    QMap<Option, QCheckBox *> mcboxOptions;
    QMap<Option, QWidget *> mwgtOptions;
    QLabel *mlblHP;
    QLabel *mlblMP;
    QLabel *mlblPAtk;
    QLabel *mlblPDef;
    QLabel *mlblAccuracy;
    QLabel *mlblCritical;
    QLabel *mlblAtkSpd;
    QLabel *mlblCP;
    QLabel *mlblWeightLimit;
    QLabel *mlblMAtk;
    QLabel *mlblMDef;
    QLabel *mlblEvasion;
    QLabel *mlblSpeed;
    QLabel *mlblCastingSpd;
    QMap<CharacterClass::Stat, QLabel *> mlblStat;
public:
    explicit CalcWidget(QWidget *parent = 0);
private:
    void addStat(QGridLayout *glt, int row, int column, CharacterClass::Stat stat);
    QMap<CharacterClass::Stat, int> calculateStats(const CharacterClass &cl,
                                                   const QMap<CharacterClass::Stat, int> &tattooModifiers);
    QMap<CharacterClass::Stat, int> calculateTattoos(const CharacterClass &cl);
    double calculateWeaponPatk();
    void createBasicParametersGroup(QHBoxLayout *hlt);
    void createBuffGroup(QVBoxLayout *vlt);
    void createCharacterParametersGroup(QHBoxLayout *hlt);
    void createCharacterStateGroup(QHBoxLayout *hlt);
    void createDebuffGroup(QVBoxLayout *vlt);
    void createEquipmentGroup(QVBoxLayout *vlt);
    void createOptionsGroup(QVBoxLayout *vlt);
    void createPassiveSkillGroup(QVBoxLayout *vlt);
    void createToggleGroup(QVBoxLayout *vlt);
    void loadBaseMP();
    void loadClasses();
    void loadData();
    void loadStats();
    void loadTattoos();
    void loadWeaponEnchantBonus();
    void loadWeapons();
    int patkEnchantBonus(Grade::GradeType grade, Weapon::Type weaponType, bool twoHanded = false, bool over = false);
    QList<int> weaponsForGrade(Grade::GradeType grade, Weapon::Type type = Weapon::NoType);
private slots:
    void calculate();
    void fillBootsCmbox();
    void fillEarring1Cmbox();
    void fillEarring2Cmbox();
    void fillGlovesCmbox();
    void fillHelmetCmbox();
    void fillLowerBodyCmbox();
    void fillNecklaceCmbox();
    void fillRing1Cmbox();
    void fillRing2Cmbox();
    void fillSaCmbox();
    void fillShieldCmbox();
    void fillUpperBodyCmbox();
    void fillWeaponCmbox();
    void retranslateUi();
    void toggleCombat(bool visible);
    void toggleEquipment(bool visible);
    void togglePassiveSkill(bool visible);
    void toggleBuff(bool visible);
    void toggleToggle(bool visible);
    void toggleDebuff(bool visible);
};

#endif // CALCWIDGET_H
