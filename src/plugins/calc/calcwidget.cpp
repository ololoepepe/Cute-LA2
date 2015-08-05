#include "calcwidget.h"

#include "characterclass.h"
#include "grade.h"
#include "specialability.h"
#include "tattoo.h"
#include "weapon.h"

#include <BApplication>
#include <BDirTools>
#include <BDynamicTranslator>
#include <BGuiTools>
#include <BTranslation>

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QDir>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QMap>
#include <QSettings>
#include <QString>
#include <QStringList>
#include <QVariant>
#include <QVBoxLayout>
#include <QWidget>

#include <cmath>

CalcWidget::EquipmentRow::EquipmentRow()
{
    cmboxEnchant = 0;
    cmboxGrade = 0;
    cmbox = 0;
}

CalcWidget::EquipmentRow::EquipmentRow(CalcWidget *self, QGridLayout *glt, int row, const BTranslation &t)
{
    lbl = new QLabel(t.translate());
    new BDynamicTranslator(lbl, "text", t);
    glt->addWidget(lbl, row, 0);
    cmboxEnchant = new QComboBox;
    foreach (int i, bRangeD(0, 20))
        cmboxEnchant->addItem("+" + QString::number(i), i);
    connect(cmboxEnchant, SIGNAL(currentIndexChanged(int)), self, SLOT(calculate()));
    glt->addWidget(cmboxEnchant, row, 1);
    cmboxGrade = new QComboBox;
    foreach (int i, bRangeD(Grade::NoGrade, Grade::SGrade)) {
        Grade::GradeType grade = enum_cast<Grade::GradeType>(i, Grade::NoGrade, Grade::SGrade);
        cmboxGrade->addItem(Grade::gradeToString(grade) + " Grade", grade);
    }
    glt->addWidget(cmboxGrade, row, 2);
    cmbox = new QComboBox;
    connect(cmbox, SIGNAL(currentIndexChanged(int)), self, SLOT(calculate()));
    glt->addWidget(cmbox, row, 3);
}

CalcWidget::CalcWidget(QWidget *parent) :
    QWidget(parent)
{
    loadData();
    //
    BTranslation t = BTranslation::translate("CalcWidget", "Calc", "windowTitle");
    new BDynamicTranslator(this, "windowTitle", t);
    setWindowTitle(t.translate());
    //
    QVBoxLayout *vlt = new QVBoxLayout(this);
    QHBoxLayout *hlt = new QHBoxLayout;
    createCharacterParametersGroup(hlt);
    createBasicParametersGroup(hlt);
    createCharacterStateGroup(hlt);
    vlt->addLayout(hlt);
    createOptionsGroup(vlt);
    createEquipmentGroup(vlt);
    createPassiveSkillGroup(vlt);
    createBuffGroup(vlt);
    createToggleGroup(vlt);
    createDebuffGroup(vlt);
    //
    connect(bApp, SIGNAL(languageChanged()), this, SLOT(retranslateUi()));
    retranslateUi();
    calculate();
}

void CalcWidget::addStat(QGridLayout *glt, int row, int column, CharacterClass::Stat stat)
{
    QLabel *lbl = new QLabel;
    mlblStat.insert(stat, lbl);
    glt->addWidget(new QLabel(CharacterClass::statToString(stat)), row, column);
    glt->addWidget(lbl, row, column + 1);
}

QMap<CharacterClass::Stat, int> CalcWidget::calculateStats(const CharacterClass &cl,
                                                           const QMap<CharacterClass::Stat, int> &tattooModifiers)
{
    QMap<CharacterClass::Stat, int> stats;
    foreach (int i, bRangeD(CharacterClass::STR, CharacterClass::MEN)) {
        CharacterClass::Stat stat = enum_cast<CharacterClass::Stat>(i, CharacterClass::NoStat, CharacterClass::MEN);
        int val = cl.baseStat(stat);
        if (tattooModifiers.contains(stat))
            val += tattooModifiers.value(stat);
        stats.insert(stat, val);
    }
    //TODO: Armor set stat modifier
    foreach (int i, bRangeD(CharacterClass::STR, CharacterClass::MEN)) {
        CharacterClass::Stat stat = enum_cast<CharacterClass::Stat>(i, CharacterClass::NoStat, CharacterClass::MEN);
        QString text = QString::number(stats.value(stat));
        if (tattooModifiers.contains(stat)) {
            int val = tattooModifiers.value(stat);
            if (val)
                text += " (" + QString((val > 0) ? "+" : "") + QString::number(val) + ")";
        }
        mlblStat.value(stat)->setText(text);
    }
    return stats;
}

QMap<CharacterClass::Stat, int> CalcWidget::calculateTattoos(const CharacterClass &cl)
{
    foreach (int i, bRangeD(0, mcmboxTattoo.size() - 1)) {
        QComboBox *cmbox = mcmboxTattoo.at(i);
        if (i >= cl.maxAllowedTattoos()) {
            cmbox->blockSignals(true);
            cmbox->setCurrentIndex(0);
            cmbox->blockSignals(false);
            cmbox->setEnabled(false);
            mcboxTattoo.at(i)->setEnabled(false);
            continue;
        } else {
            mcboxTattoo.at(i)->setEnabled(true);
            cmbox->setEnabled(true);
        }
        foreach (int j, bRangeD(1, cmbox->count() - 1)) {
            const Tattoo &tattoo = mtattoos[cmbox->itemData(j).toInt()];
            bool enabled = tattoo.increasedStat() != cl.restrictedTattoo()
                    && tattoo.decreasedStat() != cl.restrictedTattoo();
            enabled = enabled && ((cl.maxAllowedTattoos() > 2) || (tattoo.decreasedValue() - tattoo.increasedValue()));
            BGuiTools::setItemEnabled(cmbox, j, enabled);
            if (!enabled && j == cmbox->currentIndex()) {
                cmbox->blockSignals(true);
                cmbox->setCurrentIndex(0);
                cmbox->blockSignals(false);
            }
        }
    }
    QMap<CharacterClass::Stat, int> tattooModifiers;
    foreach (QComboBox *cmbox, mcmboxTattoo) {
        if (cmbox->currentIndex() <= 0 || !mcboxTattoo.at(mcmboxTattoo.indexOf(cmbox))->isChecked())
            continue;
        const Tattoo &tattoo = mtattoos[cmbox->itemData(cmbox->currentIndex()).toInt()];
        if (tattooModifiers.contains(tattoo.increasedStat())) {
            tattooModifiers[tattoo.increasedStat()] += tattoo.increasedValue();
            if (tattooModifiers[tattoo.increasedStat()] > 5)
                tattooModifiers[tattoo.increasedStat()] = 5;
        } else {
            tattooModifiers.insert(tattoo.increasedStat(), tattoo.increasedValue());
        }
        if (tattooModifiers.contains(tattoo.decreasedStat()))
            tattooModifiers[tattoo.decreasedStat()] -= tattoo.decreasedValue();
        else
            tattooModifiers.insert(tattoo.decreasedStat(), -1 * tattoo.decreasedValue());
    }
    return tattooModifiers;
}

double CalcWidget::calculateWeaponPatk()
{
    EquipmentRow row = mequipment.value(WeaponEquipment);
    int weaponId = row.cmbox->itemData(row.cmbox->currentIndex()).toInt();
    if (weaponId <= 0)
        return 4.0;
    const Weapon &w = mweapons.value(weaponId);
    double patk = w.patk();
    int enchant = row.cmboxEnchant->itemData(row.cmboxEnchant->currentIndex()).toInt();
    if (enchant > 0)
        patk += enchant * patkEnchantBonus(w.grade(), w.type(), w.twoHanded());
    if (enchant > 3)
        patk += (enchant - 3) * patkEnchantBonus(w.grade(), w.type(), w.twoHanded());
    return patk;
}

void CalcWidget::createBasicParametersGroup(QHBoxLayout *hlt)
{
    BTranslation t = BTranslation::translate("CalcWidget", "Basic parameters", "gbox title");
    QGroupBox *gbox = new QGroupBox(t.translate());
    new BDynamicTranslator(gbox, "title", t);
    QVBoxLayout *vlt = new QVBoxLayout(gbox);
    QGridLayout *glt = new QGridLayout;
    addStat(glt, 0, 0, CharacterClass::STR);
    addStat(glt, 0, 2, CharacterClass::DEX);
    addStat(glt, 0, 4, CharacterClass::CON);
    addStat(glt, 1, 0, CharacterClass::INT);
    addStat(glt, 1, 2, CharacterClass::WIT);
    addStat(glt, 1, 4, CharacterClass::MEN);
    vlt->addLayout(glt);
    t = BTranslation::translate("CalcWidget", "Tattoos", "gbox title");
    QLabel *lbl = new QLabel(t.translate());
    new BDynamicTranslator(lbl, "text", t);
    vlt->addWidget(lbl);
    foreach (int i, bRangeD(1, 3)) {
        Q_UNUSED(i)
        QHBoxLayout *hlt2 = new QHBoxLayout;
        QCheckBox *cbox = new QCheckBox;
        connect(cbox, SIGNAL(toggled(bool)), this, SLOT(calculate()));
        mcboxTattoo << cbox;
        hlt2->addWidget(cbox);
        QComboBox *cmbox = new QComboBox;
        cmbox->setMaxVisibleItems(20);
        cmbox->addItem("");
        foreach (int j, mtattoos.keys()) {
            const Tattoo &tattoo = mtattoos[j];
            QString text = CharacterClass::statToString(tattoo.increasedStat()) + "+"
                    + QString::number(tattoo.increasedValue()) + " ";
            text += CharacterClass::statToString(tattoo.decreasedStat()) + "-"
                    + QString::number(tattoo.decreasedValue());
            cmbox->addItem(text, j);
        }
        mcmboxTattoo << cmbox;
        connect(cmbox, SIGNAL(currentIndexChanged(int)), this, SLOT(calculate()));
        hlt2->addWidget(cmbox);
        vlt->addLayout(hlt2);
    }
    hlt->addWidget(gbox);
}

void CalcWidget::createBuffGroup(QVBoxLayout *vlt)
{
    //
}

void CalcWidget::createCharacterParametersGroup(QHBoxLayout *hlt)
{
    BTranslation t = BTranslation::translate("CalcWidget", "Character parameters", "gbox title");
    QGroupBox *gbox = new QGroupBox(t.translate());
    new BDynamicTranslator(gbox, "title", t);
    QHBoxLayout *hlt2 = new QHBoxLayout(gbox);
    QWidget *wgt = new QWidget;
    QFormLayout *flt = new QFormLayout(wgt);
    mcmboxLevel = new QComboBox;
    mcmboxLevel->setFixedWidth(50);
    mcmboxLevel->setMaxVisibleItems(20);
    foreach (int i, bRangeD(1, 80))
        mcmboxLevel->addItem(QString::number(i), i);
    connect(mcmboxLevel, SIGNAL(currentIndexChanged(int)), this, SLOT(calculate()));
    flt->addRow(" ", mcmboxLevel);
    mlblHP = new QLabel;
    flt->addRow(" ", mlblHP);
    mlblMP = new QLabel;
    flt->addRow(" ", mlblMP);
    mlblPAtk = new QLabel;
    flt->addRow(" ", mlblPAtk);
    mlblPDef = new QLabel;
    flt->addRow(" ", mlblPDef);
    mlblAccuracy = new QLabel;
    flt->addRow(" ", mlblAccuracy);
    mlblCritical = new QLabel;
    flt->addRow(" ", mlblCritical);
    mlblAtkSpd = new QLabel;
    flt->addRow(" ", mlblAtkSpd);
    hlt2->addWidget(wgt);
    wgt = new QWidget;
    flt = new QFormLayout(wgt);
    mcmboxClass = new QComboBox;
    mcmboxClass->setMaxVisibleItems(20);
    foreach (int i, mclasses.keys())
        mcmboxClass->addItem(mclasses.value(i).name(), i);
    connect(mcmboxClass, SIGNAL(currentIndexChanged(int)), this, SLOT(calculate()));
    flt->addRow(" ", mcmboxClass);
    mlblCP = new QLabel;
    flt->addRow(" ", mlblCP);
    mlblWeightLimit = new QLabel;
    flt->addRow(" ", mlblWeightLimit);
    mlblMAtk = new QLabel;
    flt->addRow(" ", mlblMAtk);
    mlblMDef = new QLabel;
    flt->addRow(" ", mlblMDef);
    mlblEvasion = new QLabel;
    flt->addRow(" ", mlblEvasion);
    mlblSpeed = new QLabel;
    flt->addRow(" ", mlblSpeed);
    mlblCastingSpd = new QLabel;
    flt->addRow(" ", mlblCastingSpd);
    hlt2->addWidget(wgt);
    hlt->addWidget(gbox);
}

void CalcWidget::createCharacterStateGroup(QHBoxLayout *hlt)
{
    //
}

void CalcWidget::createDebuffGroup(QVBoxLayout *vlt)
{
    //
}

void CalcWidget::createEquipmentGroup(QVBoxLayout *vlt)
{
    BTranslation t = BTranslation::translate("CalcWidget", "Equipment", "gbox title");
    QGroupBox *gbox = new QGroupBox(t.translate());
    new BDynamicTranslator(gbox, "title", t);
    QGridLayout *glt = new QGridLayout(gbox);
    EquipmentRow row(this, glt, 0, BTranslation::translate("CalcWidget", "Weapon", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillWeaponCmbox()));
    connect(row.cmbox, SIGNAL(currentIndexChanged(int)), this, SLOT(fillSaCmbox()));
    mequipment.insert(WeaponEquipment, row);
    mcmboxSA = new QComboBox;
    glt->addWidget(mcmboxSA, 0, 4);
    fillWeaponCmbox();
    t = BTranslation::translate("CalcWidget", "Select set", "lbl text");
    QLabel *lbl = new QLabel(t.translate());
    new BDynamicTranslator(lbl, "text", t);
    glt->addWidget(lbl, 1, 2);
    mcmboxArmorSet = new QComboBox;
    //TODO: Fill
    glt->addWidget(mcmboxArmorSet, 1, 3);
    row = EquipmentRow(this, glt, 2, BTranslation::translate("CalcWidget", "Shield", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillShieldCmbox()));
    mequipment.insert(ShieldEquipment, row);
    fillShieldCmbox();
    row = EquipmentRow(this, glt, 3, BTranslation::translate("CalcWidget", "Helmet", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillHelmetCmbox()));
    mequipment.insert(HelmetEquipment, row);
    fillHelmetCmbox();
    row = EquipmentRow(this, glt, 4, BTranslation::translate("CalcWidget", "Upper body", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillUpperBodyCmbox()));
    mequipment.insert(UpperBodyEquipment, row);
    fillUpperBodyCmbox();
    row = EquipmentRow(this, glt, 5, BTranslation::translate("CalcWidget", "Lower body", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillLowerBodyCmbox()));
    mequipment.insert(LowerBodyEquipment, row);
    fillLowerBodyCmbox();
    row = EquipmentRow(this, glt, 6, BTranslation::translate("CalcWidget", "Gloves", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillGlovesCmbox()));
    mequipment.insert(GlovesEquipment, row);
    fillGlovesCmbox();
    row = EquipmentRow(this, glt, 7, BTranslation::translate("CalcWidget", "Boots", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillBootsCmbox()));
    mequipment.insert(BootsEquipment, row);
    fillBootsCmbox();
    t = BTranslation::translate("CalcWidget", "Select jewelry set", "lbl text");
    lbl = new QLabel(t.translate());
    new BDynamicTranslator(lbl, "text", t);
    glt->addWidget(lbl, 8, 2);
    mcmboxJewelrySet = new QComboBox;
    //TODO: Fill
    glt->addWidget(mcmboxJewelrySet, 8, 3);
    row = EquipmentRow(this, glt, 9, BTranslation::translate("CalcWidget", "Necklace", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillNecklaceCmbox()));
    mequipment.insert(NecklaceEquipment, row);
    fillNecklaceCmbox();
    row = EquipmentRow(this, glt, 10, BTranslation::translate("CalcWidget", "Earring", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillEarring1Cmbox()));
    mequipment.insert(Earring1Equipment, row);
    fillEarring1Cmbox();
    row = EquipmentRow(this, glt, 11, BTranslation::translate("CalcWidget", "Earring", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillEarring2Cmbox()));
    mequipment.insert(Earring2Equipment, row);
    fillEarring2Cmbox();
    row = EquipmentRow(this, glt, 12, BTranslation::translate("CalcWidget", "Ring", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillRing1Cmbox()));
    mequipment.insert(Ring1Equipment, row);
    fillRing1Cmbox();
    row = EquipmentRow(this, glt, 13, BTranslation::translate("CalcWidget", "Ring", "lbl text"));
    connect(row.cmboxGrade, SIGNAL(currentIndexChanged(int)), this, SLOT(fillRing2Cmbox()));
    mequipment.insert(Ring2Equipment, row);
    fillRing2Cmbox();
    vlt->addWidget(gbox);
    mwgtOptions.insert(EquipmentOption, gbox);
}

void CalcWidget::createOptionsGroup(QVBoxLayout *vlt)
{
    BTranslation t = BTranslation::translate("CalcWidget", "Display the following", "gbox title");
    QGroupBox *gbox = new QGroupBox(t.translate());
    new BDynamicTranslator(gbox, "title", t);
    QHBoxLayout *hlt = new QHBoxLayout(gbox);
    t = BTranslation::translate("CalcWidget", "Combat calculations", "cbox text");
    QCheckBox *cbox = new QCheckBox(t);
    new BDynamicTranslator(cbox, "text", t);
    cbox->setChecked(true);
    connect(cbox, SIGNAL(toggled(bool)), this, SLOT(toggleCombat(bool)));
    hlt->addWidget(cbox);
    mcboxOptions.insert(CombatOption, cbox);
    t = BTranslation::translate("CalcWidget", "Equipment", "cbox text");
    cbox = new QCheckBox(t);
    new BDynamicTranslator(cbox, "text", t);
    cbox->setChecked(true);
    connect(cbox, SIGNAL(toggled(bool)), this, SLOT(toggleEquipment(bool)));
    hlt->addWidget(cbox);
    mcboxOptions.insert(EquipmentOption, cbox);
    t = BTranslation::translate("CalcWidget", "Passive skills", "cbox text");
    cbox = new QCheckBox(t);
    new BDynamicTranslator(cbox, "text", t);
    cbox->setChecked(true);
    connect(cbox, SIGNAL(toggled(bool)), this, SLOT(togglePassiveSkill(bool)));
    hlt->addWidget(cbox);
    mcboxOptions.insert(PassiveSkillOption, cbox);
    t = BTranslation::translate("CalcWidget", "Buffs", "cbox text");
    cbox = new QCheckBox(t);
    new BDynamicTranslator(cbox, "text", t);
    cbox->setChecked(true);
    connect(cbox, SIGNAL(toggled(bool)), this, SLOT(toggleBuff(bool)));
    hlt->addWidget(cbox);
    mcboxOptions.insert(BuffOption, cbox);
    t = BTranslation::translate("CalcWidget", "Toggles", "cbox text");
    cbox = new QCheckBox(t);
    new BDynamicTranslator(cbox, "text", t);
    cbox->setChecked(true);
    connect(cbox, SIGNAL(toggled(bool)), this, SLOT(toggleToggle(bool)));
    hlt->addWidget(cbox);
    mcboxOptions.insert(ToggleOption, cbox);
    t = BTranslation::translate("CalcWidget", "Debuffs", "cbox text");
    cbox = new QCheckBox(t);
    new BDynamicTranslator(cbox, "text", t);
    cbox->setChecked(true);
    connect(cbox, SIGNAL(toggled(bool)), this, SLOT(toggleDebuff(bool)));
    hlt->addWidget(cbox);
    mcboxOptions.insert(DebuffOption, cbox);
    vlt->addWidget(gbox);
}

void CalcWidget::createPassiveSkillGroup(QVBoxLayout *vlt)
{
    //
}

void CalcWidget::createToggleGroup(QVBoxLayout *vlt)
{
    //
}

void CalcWidget::loadBaseMP()
{
    QSettings s(":/calc/data/base-mp.conf", QSettings::IniFormat);
    foreach (int i, bRangeD(1, 6)) {
        s.beginGroup("Type" + QString::number(i));
        QMap<int, double> m;
        foreach (int lvl, bRangeD(1, 80))
            m.insert(lvl, s.value("lvl_" + QString::number(lvl)).toDouble());
        mbaseMP.insert(i, m);
        s.endGroup();
    }
}

void CalcWidget::loadClasses()
{
    int i = 0;
    foreach (const QString &fn, BDirTools::entryList(":/calc/data/classes", QStringList() << "*.conf", QDir::Files)) {
        mclasses.insert(i, CharacterClass(fn));
        ++i;
    }
}

void CalcWidget::loadData()
{
    loadClasses();
    loadBaseMP();
    loadStats();
    loadTattoos();
    loadWeaponEnchantBonus();
    loadWeapons();
}

void CalcWidget::loadStats()
{
    QSettings s(":/calc/data/stat-mod.conf", QSettings::IniFormat);
    foreach (int i, bRangeD(CharacterClass::STR, CharacterClass::MEN)) {
        CharacterClass::Stat stat = enum_cast<CharacterClass::Stat>(i, CharacterClass::NoStat, CharacterClass::MEN);
        s.beginGroup(CharacterClass::statToString(stat));
        QMap<int, double> m;
        foreach (int value, bRangeD(1, 60)) {
            double d = s.value("value_" + QString::number(value)).toDouble();
            if (qFuzzyIsNull(d))
                break;
            m.insert(value, d);
        }
        mstatModifiers.insert(stat, m);
        s.endGroup();
    }
}

void CalcWidget::loadTattoos()
{
    int i = 0;
    foreach (const QString &fn, BDirTools::entryList(":/calc/data/tattoos", QStringList() << "*.conf", QDir::Files)) {
        mtattoos.insert(i, Tattoo(fn));
        ++i;
    }
}

void CalcWidget::loadWeaponEnchantBonus()
{
    QSettings s(":/calc/data/enchant-bonus.conf", QSettings::IniFormat);
    foreach (int g, bRangeD(Grade::DGrade, Grade::SGrade)) {
        Grade::GradeType grade = enum_cast<Grade::GradeType>(g, Grade::NoGrade, Grade::SGrade);
        s.beginGroup(Grade::gradeToString(grade));
        int patk = s.value("patk/enchant").toInt();
        QMap< Weapon::Type, QMap<bool, int> > m;
        foreach (int t, bRangeD(Weapon::Blunt, Weapon::Sword)) {
            Weapon::Type type = enum_cast<Weapon::Type>(t, Weapon::NoType, Weapon::Sword);
            s.beginGroup("patk/" + QString::number(t));
            int patkType = s.value("enchant", patk).toInt();
            QMap<bool, int> mm;
            mm.insert(false, patkType);
            mm.insert(true, s.value("two_handed/enchant", patkType).toInt());
            m.insert(type, mm);
            s.endGroup();
        }
        mweaponPatkEnchantBonus.insert(grade, m);
        mweaponMatkEnchantBonus.insert(grade, s.value("matk/enchant").toInt());
        s.endGroup();
    }
}

void CalcWidget::loadWeapons()
{
    int i = 0;
    foreach (const QString &fn, BDirTools::entryList(":/calc/data/weapons", QStringList() << "*.conf", QDir::Files)) {
        mweapons.insert(i, Weapon(fn));
        ++i;
    }
}

int CalcWidget::patkEnchantBonus(Grade::GradeType grade, Weapon::Type weaponType, bool twoHanded, bool over)
{
    return mweaponPatkEnchantBonus.value(grade).value(weaponType).value(twoHanded) * (over ? 2 : 1);
}

QList<int> CalcWidget::weaponsForGrade(Grade::GradeType grade, Weapon::Type type)
{
    QList<int> list;
    foreach (int id, mweapons.keys()) {
        const Weapon &w = mweapons.value(id);
        if (w.grade() != grade)
            continue;
        if (Weapon::NoType != type && w.type() != type)
            continue;
        list << id;
    }
    return list;
}

void CalcWidget::calculate()
{
    const CharacterClass &cl = mclasses.value(mcmboxClass->itemData(mcmboxClass->currentIndex()).toInt());
    int lvl = mcmboxLevel->itemData(mcmboxLevel->currentIndex()).toInt();
    double lvlModifier = (double (lvl) + 89.0) / 100.0;
    QMap<CharacterClass::Stat, int> tattooModifiers = calculateTattoos(cl);
    QMap<CharacterClass::Stat, int> stats = calculateStats(cl, tattooModifiers);
    double baseHP = cl.baseHP(lvl) + cl.hpK1(lvl) * double(lvl) + cl.hpK2(lvl) * double(lvl) * double(lvl);
    double baseCP = baseHP * cl.cpK();
    double baseMP = mbaseMP.value(cl.classType(lvl)).value(lvl);
    double hp = baseHP * mstatModifiers.value(CharacterClass::CON).value(stats.value(CharacterClass::CON));
    if (hp < 1.0)
        hp = 1.0;
    double cp = baseCP * mstatModifiers.value(CharacterClass::CON).value(stats.value(CharacterClass::CON));
    double mp = baseMP * mstatModifiers.value(CharacterClass::MEN).value(stats.value(CharacterClass::MEN));
    double weaponPatk = calculateWeaponPatk();
    double patk = weaponPatk;
    patk *= mstatModifiers.value(CharacterClass::STR).value(stats.value(CharacterClass::STR));
    patk *= lvlModifier;
    //*NECKLACEPATK
    //*MasteryPATK
    //*BuffPATK
    //+AddPATK;
    mlblPAtk->setText(QString::number(floor(patk)));
    //
    mlblHP->setText(QString::number(floor(hp)));
    mlblMP->setText(QString::number(floor(mp)));
    mlblCP->setText(QString::number(floor(cp)));
}

void CalcWidget::fillBootsCmbox()
{
    //
}

void CalcWidget::fillEarring1Cmbox()
{
    //
}

void CalcWidget::fillEarring2Cmbox()
{
    //
}

void CalcWidget::fillGlovesCmbox()
{
    //
}

void CalcWidget::fillHelmetCmbox()
{
    //
}

void CalcWidget::fillLowerBodyCmbox()
{
    //
}

void CalcWidget::fillNecklaceCmbox()
{
    //
}

void CalcWidget::fillRing1Cmbox()
{
    //
}

void CalcWidget::fillRing2Cmbox()
{
    //
}

void CalcWidget::fillSaCmbox()
{
    EquipmentRow row = mequipment.value(WeaponEquipment);
    int id = row.cmbox->itemData(row.cmbox->currentIndex()).toInt();
    mcmboxSA->clear();
    mcmboxSA->addItem(tr("No", "cmbox item text"));
    if (0 == id) {
        mcmboxSA->setEnabled(false);
        return;
    }
    const Weapon &w = mweapons.value(id);
    if (w.sa().isEmpty()) {
        mcmboxSA->setEnabled(false);
        return;
    }
    mcmboxSA->setEnabled(true);
    foreach (const SpecialAbility &sa, w.sa())
        mcmboxSA->addItem(SpecialAbility::saTypeToString(sa.type()), sa.type());
}

void CalcWidget::fillShieldCmbox()
{
    //
}

void CalcWidget::fillUpperBodyCmbox()
{
    //
}

void CalcWidget::fillWeaponCmbox()
{
    EquipmentRow row = mequipment.value(WeaponEquipment);
    QComboBox *cmbox = row.cmbox;
    cmbox->clear();
    cmbox->addItem(tr("No", "cmbox item text"));
    Grade::GradeType grade = enum_cast<Grade::GradeType>(row.cmboxGrade->itemData(row.cmboxGrade->currentIndex()),
                                                         Grade::NoGrade, Grade::SGrade);
    foreach (int i, bRangeD(Weapon::Blunt, Weapon::Sword)) {
        Weapon::Type type = enum_cast<Weapon::Type>(i, Weapon::NoType, Weapon::Sword);
        QList<int> list = weaponsForGrade(grade, type);
        if (!list.isEmpty()) {
            cmbox->addItem("==== " + Weapon::weaponTypeToString(type) + "====");
            BGuiTools::setItemEnabled(cmbox, cmbox->count() - 1, false);
            foreach (int id, list) {
                const Weapon &w = mweapons.value(id);
                cmbox->addItem(QString::number(w.patk()) + "/" + QString::number(w.matk()) + " " + w.name(), id);
            }
        }
    }
}

void CalcWidget::retranslateUi()
{
    int indSA = mcmboxSA->currentIndex();
    int indWeapon = mequipment.value(WeaponEquipment).cmbox->currentIndex();
    fillWeaponCmbox();
    mequipment.value(WeaponEquipment).cmbox->setCurrentIndex(indWeapon);
    fillSaCmbox();
    mcmboxSA->setCurrentIndex(indSA);
    BGuiTools::labelForField<QLabel>(mcmboxLevel)->setText(tr("Lv.", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblHP)->setText(tr("HP", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblMP)->setText(tr("MP", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblPAtk)->setText(tr("P.Atk.", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblPDef)->setText(tr("P.Def.", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblAccuracy)->setText(tr("Accuracy", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblCritical)->setText(tr("Critical", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblAtkSpd)->setText(tr("Atk.Spd.", "lbl text"));
    BGuiTools::labelForField<QLabel>(mcmboxClass)->setText(tr("Class", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblCP)->setText(tr("CP", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblWeightLimit)->setText(tr("Weight limit", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblMAtk)->setText(tr("M.Atk.", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblMDef)->setText(tr("M.Def.", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblEvasion)->setText(tr("Evasion", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblSpeed)->setText(tr("Speed", "lbl text"));
    BGuiTools::labelForField<QLabel>(mlblCastingSpd)->setText(tr("Casting Spd.", "lbl text"));
    foreach (QComboBox *cmbox, mcmboxTattoo)
        cmbox->setItemText(0, tr("No", "cmbox item text"));
}

void CalcWidget::toggleCombat(bool visible)
{
    //
}

void CalcWidget::toggleEquipment(bool visible)
{
    mwgtOptions.value(EquipmentOption)->setVisible(visible);
}

void CalcWidget::togglePassiveSkill(bool visible)
{
    //
}

void CalcWidget::toggleBuff(bool visible)
{
    //
}

void CalcWidget::toggleToggle(bool visible)
{
    //
}

void CalcWidget::toggleDebuff(bool visible)
{
    //
}
