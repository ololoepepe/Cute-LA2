#include "craftwidget.h"
#include "global.h"

#include <BTranslation>
#include <BApplication>

#include <QWidget>
#include <QTimer>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>

CraftWidget::CraftWidget(QWidget *parent) :
    QWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    trStart = BTranslation::translate("CraftWidget", "Start crafting", "btn text");
    trStop = BTranslation::translate("CraftWidget", "Stop crafting", "btn text");
    //
    QVBoxLayout *vlt = new QVBoxLayout(this);
      QHBoxLayout *hlt = new QHBoxLayout;
        btn = new QPushButton;
          connect(btn, SIGNAL(clicked()), this, SLOT(btnClicked()));
        hlt->addWidget(btn);
      vlt->addLayout(hlt);
        QFormLayout *flt = new QFormLayout;
          sboxRegen = new QSpinBox;
            sboxRegen->setMinimum(1);
            sboxRegen->setMaximum(100);
            sboxRegen->setValue(Global::mpRegen());
            connect(sboxRegen, SIGNAL(valueChanged(int)), this, SLOT(sboxRegenValueChanged(int)));
          flt->addRow(" ", sboxRegen);
          sboxConsumption = new QSpinBox;
            sboxConsumption->setMinimum(10);
            sboxConsumption->setMaximum(500);
            sboxConsumption->setValue(Global::mpConsumption());
            connect(sboxConsumption, SIGNAL(valueChanged(int)), this, SLOT(sboxConsumptionValueChanged(int)));
          flt->addRow(" ", sboxConsumption);
          hltButton = new QHBoxLayout;
            lblButtonPos = new QLabel;
              QPoint pos = Global::craftButtonPos();
              lblButtonPos->setText("<b>(" + QString::number(pos.x()) + "; " + QString::number(pos.y()) +")</b>");
            hltButton->addWidget(lblButtonPos);
            btnSelectButton = new QPushButton;
              connect(btnSelectButton, SIGNAL(clicked()), this, SLOT(selectButton()));
            hltButton->addWidget(btnSelectButton);
          flt->addRow(" ", hltButton);
        vlt->addLayout(flt);
    //
    retranslateUi();
    connect(bApp, SIGNAL(languageChanged()), this, SLOT(retranslateUi()));
}

int CraftWidget::calculateTimeout()
{
    return (Global::mpConsumption() / Global::mpRegen()) * 3 * BeQt::Second;
}

void CraftWidget::retranslateUi()
{
    BApplication::labelForField<QLabel>(sboxRegen)->setText(tr("MP regeneration per tick:", "lbl text"));
    BApplication::labelForField<QLabel>(sboxConsumption)->setText(tr("MP consumption per item:", "lbl text"));
    BApplication::labelForField<QLabel>(hltButton)->setText(tr("Craft button:", "lbl text"));
    btnSelectButton->setText(tr("Select position", "btn text"));
    btn->setText(timer.isActive() ? trStop : trStart);
}

void CraftWidget::timeout()
{
    Global::emulateMouseClick(Qt::LeftButton, Global::craftButtonPos());
    timer.setInterval(calculateTimeout());
}

void CraftWidget::btnClicked()
{
    Global::switchToWindow();
    if (timer.isActive())
    {
        timer.stop();
        btn->setText(trStart);
    }
    else
    {
        timer.start(calculateTimeout());
        btn->setText(trStop);
    }
}

void CraftWidget::selectButton()
{
    if (!Global::selectCraftButtonPos())
        return;
    QPoint pos = Global::craftButtonPos();
    lblButtonPos->setText("<b>(" + QString::number(pos.x()) + "; " + QString::number(pos.y()) +")</b>");
}

void CraftWidget::sboxRegenValueChanged(int value)
{
    Global::setMpRegen(value);
}

void CraftWidget::sboxConsumptionValueChanged(int value)
{
    Global::setMpConsumption(value);
}
