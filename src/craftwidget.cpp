#include "craftwidget.h"

#include "application.h"
#include "global.h"

#include <BGuiTools>
#include <BTranslation>

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include <climits>

CraftWidget::CraftWidget(QWidget *parent) :
    QWidget(parent)
{
    active = false;
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
          sboxCount = new QSpinBox;
            sboxCount->setMinimum(-1);
            sboxCount->setMaximum(INT_MAX);
            sboxCount->setValue(-1);
          flt->addRow(" ", sboxCount);
          sboxDelay = new QSpinBox;
            sboxDelay->setMinimum(1);
            sboxDelay->setMaximum(60);
            sboxDelay->setValue(Global::craftStartDelay());
            connect(sboxDelay, SIGNAL(valueChanged(int)), this, SLOT(sboxDelayValueChanged(int)));
          flt->addRow(" ", sboxDelay);
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
    double d = double(Global::mpConsumption()) / double(Global::mpRegen());
    if (d < 0.1)
        d = 0.1;
    return int(d) * 3 * BeQt::Second;
}

void CraftWidget::retranslateUi()
{
    BGuiTools::labelForField<QLabel>(sboxCount)->setText(tr("Item count:", "lbl text"));
    BGuiTools::labelForField<QLabel>(sboxDelay)->setText(tr("Start delay (seconds):", "lbl text"));
    BGuiTools::labelForField<QLabel>(sboxRegen)->setText(tr("MP regeneration per tick:", "lbl text"));
    BGuiTools::labelForField<QLabel>(sboxConsumption)->setText(tr("MP consumption per item:", "lbl text"));
    BGuiTools::labelForField<QLabel>(hltButton)->setText(tr("Craft button:", "lbl text"));
    btnSelectButton->setText(tr("Select position", "btn text"));
    btn->setText(timer.isActive() ? trStop : trStart);
}

void CraftWidget::timeout()
{
    Global::emulateMouseClick(Qt::LeftButton, Global::craftButtonPos());
    if (sboxCount->value()) {
        timer.setInterval(calculateTimeout());
        sboxCount->setValue(sboxCount->value() - 1);
    } else {
        btn->animateClick();
    }
}

void CraftWidget::btnClicked()
{
    active = !active;
    btn->setText(active ? trStop : trStart);
    if (!active) {
        timer.stop();
        Global::switchToWindow();
        return;
    }
    if (!sboxCount->value())
        sboxCount->setValue(-1);
    int delay = Global::craftStartDelay();
    Global::switchToWindow();
    BeQt::waitNonBlocking(btn, SIGNAL(clicked()), delay * BeQt::Second);
    if (!active)
        return;
    timeout();
    timer.start(calculateTimeout());
}

void CraftWidget::selectButton()
{
    if (!Global::selectCraftButtonPos())
        return;
    QPoint pos = Global::craftButtonPos();
    lblButtonPos->setText("<b>(" + QString::number(pos.x()) + "; " + QString::number(pos.y()) +")</b>");
}

void CraftWidget::sboxDelayValueChanged(int value)
{
    Global::setCraftStartDelay(value);
}

void CraftWidget::sboxRegenValueChanged(int value)
{
    Global::setMpRegen(value);
}

void CraftWidget::sboxConsumptionValueChanged(int value)
{
    Global::setMpConsumption(value);
}
