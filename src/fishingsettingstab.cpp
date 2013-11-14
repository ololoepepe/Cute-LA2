#include "fishingsettingstab.h"
#include "application.h"
#include "global.h"
#include "mainwindow.h"

#include <BAbstractSettingsTab>

#include <QObject>
#include <QFormLayout>
#include <QString>
#include <QIcon>
#include <QComboBox>
#include <QStringList>
#include <QCheckBox>
#include <QSpinBox>

#include <QDebug>

/*============================== Public constructors =======================*/

FishingSettingsTab::FishingSettingsTab()
{
    QStringList list;
    for (int i = 1; i <= 12; ++i)
        list << "F" + QString::number(i);
    QFormLayout *flt = new QFormLayout(this);
      cboxEquip = new QCheckBox;
        cboxEquip->setText(tr("Equip fishing gear before fishing", "cbox text"));
        cboxEquip->setChecked(Global::fishingEquipBeforeStart());
      flt->addRow(tr("Equip gear:", "lbl text"), cboxEquip);
      sboxDelay = new QSpinBox;
        sboxDelay->setMinimum(1);
        sboxDelay->setMaximum(60);
        sboxDelay->setValue(Global::fishingStartDelay());
      flt->addRow(tr("Start delay (seconds):", "lbl text"), sboxDelay);
      cmboxPanel = new QComboBox;
        for (int i = 1; i <= 10; ++i)
            cmboxPanel->addItem(QString::number(i));
        cmboxPanel->setCurrentIndex(Global::fishingPanelNumber() - 1);
      flt->addRow(tr("Fishing panel number:", "lbl text"), cmboxPanel);
      cmboxMainPanel = new QComboBox;
        for (int i = 1; i <= 10; ++i)
            cmboxMainPanel->addItem(QString::number(i));
        cmboxMainPanel->setCurrentIndex(Global::mainPanelNumber() - 1);
      flt->addRow(tr("Main panel number:", "lbl text"), cmboxMainPanel);
      Global::FishingKeyList keys = Global::fishingKeyList();
      for (int i = 0; i < 9; ++i)
      {
          QComboBox *cmbox = new QComboBox;
            cmbox->addItem("---");
            cmbox->addItems(list);
            if (i < keys.size())
                cmbox->setCurrentIndex(keys.at(i));
            connect(cmbox, SIGNAL(currentIndexChanged(int)), this, SLOT(cmboxCurrentIndexChanged(int)));
          cmboxes << cmbox;
      }
      flt->addRow(tr("Rod:", "lbl text"), cmboxes.at(0));
      flt->addRow(tr("Bait:", "lbl text"), cmboxes.at(1));
      flt->addRow(tr("Fishing:", "lbl text"), cmboxes.at(2));
      flt->addRow(tr("Reeling:", "lbl text"), cmboxes.at(3));
      flt->addRow(tr("Pumping:", "lbl text"), cmboxes.at(4));
      flt->addRow(tr("Fishing shot:", "lbl text"), cmboxes.at(5));
      flt->addRow(tr("Attack:", "lbl text"), cmboxes.at(6));
      flt->addRow(tr("Sit/stand:", "lbl text"), cmboxes.at(7));
      flt->addRow(tr("Weapon:", "lbl text"), cmboxes.at(8));
}

/*============================== Public methods ============================*/

QString FishingSettingsTab::title() const
{
    return tr("Fishing", "title");
}

QIcon FishingSettingsTab::icon() const
{
    return BApplication::icon("fishing");
}

bool FishingSettingsTab::hasDefault() const
{
    return true;
}

bool FishingSettingsTab::restoreDefault()
{
    sboxDelay->setValue(5);
    return true;
}

bool FishingSettingsTab::saveSettings()
{
    Global::setFishingEquipBeforeStart(cboxEquip->isChecked());
    Global::setFishingStartDelay(sboxDelay->value());
    Global::setFishingPanelNumber(cmboxPanel->currentText().toInt());
    Global::setMainPanelNumber(cmboxMainPanel->currentText().toInt());
    Global::FishingKeyList list;
    foreach (QComboBox *c, cmboxes)
        list << c->currentText().mid(1).toInt();
    Global::setFishingKeyList(list);
    return true;
}

void FishingSettingsTab::cmboxCurrentIndexChanged(int index)
{
    Q_UNUSED(index)
    QComboBox *cmbox = qobject_cast<QComboBox *>(sender());
    QString s = cmbox->currentText();
    if (s == "---")
        return;
    foreach (QComboBox *c, cmboxes)
    {
        if (c != cmbox && s == c->currentText())
        {
            c->blockSignals(true);
            c->setCurrentIndex(0);
            c->blockSignals(false);
        }
    }
}
