#include "application.h"
#include "generalsettingstab.h"
#include "manorsettingstab.h"
#include "fishingsettingstab.h"
#include "global.h"

#include <BApplication>
#include <BSettingsDialog>
#include <BPasswordWidget>
#include <BAbstractSettingsTab>
#include <BLocaleComboBox>
#include <BDialog>

#include <QObject>
#include <QVariantMap>
#include <QByteArray>
#include <QList>
#include <QMessageBox>
#include <QApplication>
#include <QPushButton>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QSize>
#include <QPushButton>
#include <QFontComboBox>
#include <QSpinBox>
#include <QComboBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QSettings>

#include <QDebug>

/*============================================================================
================================ Application =================================
============================================================================*/

/*============================== Public constructors =======================*/

Application::Application() :
    BApplication()
{
    Global::loadSettings();
}

Application::~Application()
{
    Global::saveSettings();
}

/*============================== Protected methods =========================*/

QList<BAbstractSettingsTab *> Application::createSettingsTabs() const
{
    QList<BAbstractSettingsTab *> list;
    list << new GeneralSettingsTab;
    list << new ManorSettingsTab;
    list << new FishingSettingsTab;
    return list;
}
