#include "manorsettingstab.h"
#include "global.h"
#include "mainwindow.h"

#include <BAbstractSettingsTab>
#include <BPasswordWidget>
#include <BDirTools>

#include <QObject>
#include <QVariantMap>
#include <QString>
#include <QIcon>
#include <QCheckBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QVariant>
#include <QByteArray>
#include <QSettings>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QToolButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QFileDialog>
#include <QImage>

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
      btnGrablympiadMessage = new QPushButton(tr("Grab olympiad message", "btn text"));
        connect(btnGrablympiadMessage, SIGNAL(clicked()), this, SLOT(grabOlympiadMessage()));
      flt->addRow(tr("Olympiad message:", "lbl text"), btnGrablympiadMessage);
}

/*============================== Public methods ============================*/

QString ManorSettingsTab::title() const
{
    return tr("Manor", "title");
}

QIcon ManorSettingsTab::icon() const
{
    return BApplication::icon("harvester");
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
    MainWindow::reloadInfo(MainWindow::ManorInfo);
    return true;
}

/*============================== Private slots =============================*/

void ManorSettingsTab::grabOlympiadMessage()
{
    QImage img = Global::grabOlympiadMessage();
    if (img.isNull())
        return;
    QString fn = BApplication::location(BApplication::DataPath, BApplication::UserResources) + "/olympiad_message.png";
    if (!img.save(fn, "png"))
        return;
    MainWindow::reloadInfo(MainWindow::OlympiadMessageInfo);
}
