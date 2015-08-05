#include "calc.h"

#include "calcwidget.h"

#include <BApplication>
#include <BDirTools>
#include <BDynamicTranslator>
#include <BeQt>
#include <BPluginWrapper>
#include <BTranslation>
#include <BVersion>

#include <QAction>
#include <QDebug>
#include <QList>
#include <QMenu>
#include <QPixmap>
#include <QString>
#include <QSystemTrayIcon>
#include <QtPlugin>

Calc::Calc()
{
    mact = 0;
    msep = 0;
    mwgt = 0;
}

Calc::~Calc()
{
    //
}

void Calc::activate()
{
    BApplication::installBeqtTranslator("calc");
    mwgt = new CalcWidget;
    QMenu *mnu = BApplication::trayIcon()->contextMenu();
    BTranslation t = BTranslation::translate("Calc", "Show calc", "act text");
    QAction *act = mnu->actions().last();
    mnu->insertSeparator(act);
    mact = mnu->addAction(t.translate(), this, SLOT(showCalc()));
    new BDynamicTranslator(mact, "text", t);
    mnu->insertAction(act, mact);
}

BAboutDialog *Calc::createAboutDialog()
{
    return 0;
}

QList<BAbstractSettingsTab *> Calc::createSettingsTabs()
{
    return QList<BAbstractSettingsTab *>();
}

void Calc::deactivate()
{
    delete mact;
    delete msep;
    delete mwgt;
    mact = 0;
    msep = 0;
    mwgt = 0;
}

QString Calc::helpIndex() const
{
    return "";
}

QStringList Calc::helpSearchPaths() const
{
    return QStringList();
}

QString Calc::id() const
{
    return type() + "/calc";
}

Calc::PluginInfo Calc::info() const
{
    PluginInfo pi;
    pi.organization = "Andrey Bogdanov";
    pi.copyrightYears = "2014";
    pi.website = "https://github.com/ololoepepe/Cute-LA2";
    pi.descriptionFileName = ":/calc/description/DESCRIPTION.txt";
    pi.changeLogFileName = ":/calc/changelog/ChangeLog.txt";
    pi.licenseFileName = ":/calc/copying/COPYING.txt";
    pi.authorsFileName = ":/calc/infos/authors.beqt-info";
    pi.translatorsFileName = ":/calc/infos/translators.beqt-info";
    return pi;
}

QPixmap Calc::pixmap() const
{
    return QPixmap();
}

bool Calc::prefereStaticInfo() const
{
    return false;
}

void Calc::processStandardAboutDialog(BAboutDialog *) const
{
    //
}

Calc::StaticPluginInfo Calc::staticInfo() const
{
    return StaticPluginInfo();
}

QString Calc::title() const
{
    return tr("Calc", "title");
}

QString Calc::type() const
{
    return "any";
}

BVersion Calc::version() const
{
    return BVersion(0, 1, 0);
}

void Calc::showCalc()
{
    if (mwgt->isVisible()) {
        mwgt->activateWindow();
        mwgt->raise();
    } else {
        mwgt->show();
    }
}

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
Q_EXPORT_PLUGIN2(calc, Calc)
#endif
