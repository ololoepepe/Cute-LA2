#include "manorsettingstab.h"

#include "application.h"
#include "global.h"
#include "mainwindow.h"

#include <BAbstractSettingsTab>
#include <BDirTools>

#include <QCheckBox>
#include <QDebug>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QObject>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QTime>
#include <QTimeEdit>

/*============================================================================
================================ ManorSettingsTab ============================
============================================================================*/

/*============================== Public constructors =======================*/

ManorSettingsTab::ManorSettingsTab()
{
    QFormLayout *flt = new QFormLayout(this);
      sboxOlympiadCheckInterval = new QSpinBox;
        sboxOlympiadCheckInterval->setMinimum(1);
        sboxOlympiadCheckInterval->setMaximum(1000);
        sboxOlympiadCheckInterval->setSingleStep(10);
        sboxOlympiadCheckInterval->setValue(Global::olympiadCheckInterval());
      flt->addRow(tr("Olympiad check interval (ms):", "lbl text"), sboxOlympiadCheckInterval);
      sboxTimerInterval = new QSpinBox;
        sboxTimerInterval->setMinimum(1);
        sboxTimerInterval->setMaximum(1000);
        sboxTimerInterval->setSingleStep(50);
        sboxTimerInterval->setValue(Global::manorTimerInterval());
      flt->addRow(tr("Timer interval (ms):", "lbl text"), sboxTimerInterval);
      sboxChatRowCount = new QSpinBox;
        sboxChatRowCount->setMinimum(1);
        sboxChatRowCount->setMaximum(28);
        sboxChatRowCount->setValue(Global::chatRowCount());
      flt->addRow(tr("Chat rows to check:", "lbl text"), sboxChatRowCount);
      QHBoxLayout *hlt = new QHBoxLayout;
        cboxStartAuto = new QCheckBox;
          cboxStartAuto->setChecked(Global::manorAutoStartEnabled());
        hlt->addWidget(cboxStartAuto);
        tmedtStartAuto = new QTimeEdit;
          tmedtStartAuto->setDisplayFormat("hh:mm");
          tmedtStartAuto->setTime(Global::manorAutoStartTime());
        hlt->addWidget(tmedtStartAuto);
      flt->addRow(tr("Activate automatically:", "lbl text"), hlt);
      sboxTimeCorrection = new QSpinBox;
        sboxTimeCorrection->setMinimum(-10000);
        sboxTimeCorrection->setMaximum(10000);
        sboxTimeCorrection->setSingleStep(1);
        sboxTimeCorrection->setValue(Global::manorTimeCorrection());
      flt->addRow(tr("Correction (ms):", "lbl text"), sboxTimeCorrection);
      hlt = new QHBoxLayout;
        lblOlympiadMessageTemplate = new QLabel;
          lblOlympiadMessageTemplate->setPixmap(QPixmap::fromImage(*Global::olympiadMessageMask()));
        hlt->addWidget(lblOlympiadMessageTemplate);
        btnGrabOlympiadMessage = new QPushButton(tr("Grab message", "btn text"));
          connect(btnGrabOlympiadMessage, SIGNAL(clicked()), this, SLOT(grabOlympiadMessage()));
        hlt->addWidget(btnGrabOlympiadMessage);
      flt->addRow(tr("Olympiad message:", "lbl text"), hlt);
      hlt = new QHBoxLayout;
        lblManorButtonPos = new QLabel;
          QPoint pos = Global::manorButtonPos();
          lblManorButtonPos->setText("<b>(" + QString::number(pos.x()) + "; " + QString::number(pos.y()) +")</b>");
        hlt->addWidget(lblManorButtonPos);
        btnSelectManorButton = new QPushButton(tr("Select position", "btn text"));
          connect(btnSelectManorButton, SIGNAL(clicked()), this, SLOT(selectManorButton()));
        hlt->addWidget(btnSelectManorButton);
      flt->addRow(tr("Manor button:", "lbl text"), hlt);
}

/*============================== Public methods ============================*/

QString ManorSettingsTab::id() const
{
    return "manor";
}

QString ManorSettingsTab::title() const
{
    return tr("Manor", "title");
}

QIcon ManorSettingsTab::icon() const
{
    return Application::icon("harvester");
}

bool ManorSettingsTab::hasDefault() const
{
    return true;
}

bool ManorSettingsTab::restoreDefault()
{
    sboxOlympiadCheckInterval->setValue(10);
    sboxTimerInterval->setValue(50);
    sboxChatRowCount->setValue(4);
    return true;
}

bool ManorSettingsTab::saveSettings()
{
    Global::setOlympiadCheckInterval(sboxOlympiadCheckInterval->value());
    Global::setManorTimerInterval(sboxTimerInterval->value());
    Global::setChatRowCount(sboxChatRowCount->value());
    Global::setManorAutoStartEnabled(cboxStartAuto->isChecked());
    Global::setManorAutoStartTime(tmedtStartAuto->time());
    Global::setManorTimeCorrection(sboxTimeCorrection->value());
    return true;
}

/*============================== Private slots =============================*/

void ManorSettingsTab::grabOlympiadMessage()
{
    if (!Global::grabOlympiadMessage())
        return;
    lblOlympiadMessageTemplate->setPixmap(QPixmap::fromImage(*Global::olympiadMessageMask()));
}

void ManorSettingsTab::selectManorButton()
{
    if (!Global::selectManorButtonPos())
        return;
    QPoint pos = Global::manorButtonPos();
    lblManorButtonPos->setText("<b>(" + QString::number(pos.x()) + "; " + QString::number(pos.y()) +")</b>");
}
