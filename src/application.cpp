#include "application.h"

#include "fishingsettingstab.h"
#include "generalsettingstab.h"
#include "global.h"
#include "mainwindow.h"
#include "manorsettingstab.h"

#include <BAboutDialog>
#include <BAbstractSettingsTab>
#include <BApplication>
#include <BDirTools>
#include <BGuiTools>

#include <QDebug>
#include <QList>
#include <QObject>
#include <QTimer>

/*============================================================================
================================ Application =================================
============================================================================*/

/*============================== Public constructors =======================*/

Application::Application(int &argc, char **argv, const QString &applicationName, const QString &organizationName) :
    BApplication(argc, argv, applicationName, organizationName)
{
#if defined(BUILTIN_RESOURCES)
    Q_INIT_RESOURCE(cute_la2);
    Q_INIT_RESOURCE(cute_la2_translations);
#endif
    setApplicationVersion("0.1.0-pa");
    setOrganizationDomain("https://github.com/ololoepepe/Cute-LA2");
    setApplicationCopyrightPeriod("2013-2014");
    QFont fnt = font();
    fnt.setPointSize(10);
    setFont(fnt);
    setThemedIconsEnabled(false);
    setPreferredIconFormats(QStringList() << "png");
    setWindowIcon(icon("cute-la2"));
    installBeqtTranslator("qt");
    installBeqtTranslator("beqt");
    installBeqtTranslator("cute-la2");
    BAboutDialog::setDefaultMinimumSize(800, 400);
    setApplicationDescriptionFile(findResource("description", BDirTools::GlobalOnly) + "/DESCRIPTION.txt");
    setApplicationChangeLogFile(findResource("changelog", BDirTools::GlobalOnly) + "/ChangeLog.txt");
    setApplicationLicenseFile(findResource("copying", BDirTools::GlobalOnly) + "/COPYING.txt");
    setApplicationAuthorsFile(findResource("infos/authors.beqt-info", BDirTools::GlobalOnly));
    setApplicationTranslationsFile(findResource("infos/translators.beqt-info", BDirTools::GlobalOnly));
    setApplicationThanksToFile(findResource("infos/thanks-to.beqt-info", BDirTools::GlobalOnly));
    aboutDialogInstance()->setupWithApplicationData();
    setHelpBrowserDefaultGeometry(BGuiTools::centerOnScreenGeometry(1000, 800, 100, 50));
    Global::loadSettings();
    mmainWindow = new MainWindow;
    QTimer::singleShot(0, mmainWindow, SLOT(show()));
}

Application::~Application()
{
    delete mmainWindow;
    Global::saveSettings();
#if defined(BUILTIN_RESOURCES)
    Q_CLEANUP_RESOURCE(cute_la2);
    Q_CLEANUP_RESOURCE(cute_la2_translations);
#endif
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
