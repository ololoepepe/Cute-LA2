#include "generalsettingstab.h"
#include "application.h"
#include "global.h"
#include "mainwindow.h"

#include <BAbstractSettingsTab>
#include <BLocaleComboBox>

#include <QObject>
#include <QFormLayout>
#include <QString>
#include <QIcon>
#include <QLocale>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QSpinBox>
#include <QCheckBox>

/*============================================================================
================================ GeneralSettingsTab ==========================
============================================================================*/

/*============================== Public constructors =======================*/

GeneralSettingsTab::GeneralSettingsTab() :
    BAbstractSettingsTab()
{
    QFormLayout *flt = new QFormLayout(this);
    mlcmbox = new BLocaleComboBox(true, this);
      mlcmbox->setCurrentLocale(Application::locale());
    flt->addRow("Language (" + tr("language", "lbl text") + "):", mlcmbox);
    QHBoxLayout *hlt = new QHBoxLayout;
      ledtGameDir = new QLineEdit;
        ledtGameDir->setReadOnly(true);
        ledtGameDir->setText(Global::gameDir());
      hlt->addWidget(ledtGameDir);
      QPushButton *btn = new QPushButton(tr("Search...", "btn text"));
        connect(btn, SIGNAL(clicked()), this, SLOT(selectGameDir()));
      hlt->addWidget(btn);
    flt->addRow(tr("Game dir:", "lbl text"), hlt);
    sboxTimerInterval = new QSpinBox;
      sboxTimerInterval->setMinimum(1);
      sboxTimerInterval->setMaximum(1000);
      sboxTimerInterval->setSingleStep(10);
      sboxTimerInterval->setValue(Global::timerInterval());
    flt->addRow(tr("Timer interval (ms):", "lbl text"), sboxTimerInterval);
    sboxDelay = new QSpinBox;
      sboxDelay->setMinimum(1);
      sboxDelay->setMaximum(60);
      sboxDelay->setValue(Global::detectionDelay());
    flt->addRow(tr("Detection delay (seconds):", "lbl text"), sboxDelay);
    cboxDetectWindow = new QCheckBox;
      cboxDetectWindow->setChecked(Global::detectWindowID());
    flt->addRow(tr("Detect window ID:", "lbl text"), cboxDetectWindow);
}

/*============================== Public methods ============================*/

QString GeneralSettingsTab::title() const
{
    return tr("General", "title");
}

QIcon GeneralSettingsTab::icon() const
{
    return Application::icon("configure");
}

bool GeneralSettingsTab::hasDefault() const
{
    return true;
}

bool GeneralSettingsTab::restoreDefault()
{
    sboxTimerInterval->setValue(100);
    sboxDelay->setValue(5);
    return true;
}

bool GeneralSettingsTab::saveSettings()
{
    Application::setLocale(mlcmbox->currentLocale());
    Global::setGameDir(ledtGameDir->text());
    Global::setTimerInterval(sboxTimerInterval->value());
    Global::setDetectionDelay(sboxDelay->value());
    Global::setDetectWindowID(cboxDetectWindow->isChecked());
    MainWindow::reloadInfo(MainWindow::GeneralInfo);
    return true;
}

/*============================== Private slots =============================*/

void GeneralSettingsTab::selectGameDir()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select directory", "caption"), ledtGameDir->text());
    if (dir.isEmpty())
        return;
    ledtGameDir->setText(dir);
}
