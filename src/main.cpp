#include "application.h"
#include "mainwindow.h"
#include "global.h"

#include <BVersion>
#include <BApplication>
#include <BDirTools>
#include <BTranslator>
#include <BLogger>
#include <BAboutDialog>
#include <BApplicationServer>

#include <QObject>
#include <QString>
#include <QStringList>
#include <QTextCodec>
#include <QApplication>
#include <QIcon>
#include <QDir>
#include <QFont>
#include <QPixmap>
#include <QHash>
#include <QSettings>

#include <QDebug>

static QString resource(const QString &subpath)
{
    return BDirTools::findResource(subpath, BDirTools::GlobalOnly);
}

int main(int argc, char **argv)
{
    bRegister();
    QApplication app(argc, argv);
    QApplication::setApplicationName("Cute LA2");
    QApplication::setApplicationVersion("0.1.0-pa");
    QApplication::setOrganizationName("Andrey Bogdanov");
    QApplication::setOrganizationDomain("https://github.com/the-dark-angel/Cute-LA2");
    QFont fnt = QApplication::font();
    fnt.setPointSize(10);
    QApplication::setFont(fnt);
    QString home = QDir::home().dirName();
    BApplicationServer s(9970 + qHash(home) % 10, QCoreApplication::applicationName() + "0" + home);
    int ret = 0;
    if ( !s.testServer() )
    {
        s.listen();
#if defined(BUILTIN_RESOURCES)
        Q_INIT_RESOURCE(cute_la2);
        Q_INIT_RESOURCE(cute_la2_translations);
#endif
        Application bapp;
        Q_UNUSED(bapp)
        BApplication::setThemedIconsEnabled(false);
        BApplication::setPreferredIconFormats(QStringList() << "png");
        QApplication::setWindowIcon(BApplication::icon("cute-la2"));
        BApplication::installTranslator(new BTranslator("qt"));
        BApplication::installTranslator(new BTranslator("beqt"));
        BApplication::installTranslator(new BTranslator("cute-la2"));
        BAboutDialog::setDefaultMinimumSize(800, 400);
        BApplication::setApplicationCopyrightPeriod("2013");
        BApplication::setApplicationDescriptionFile(resource("description") + "/DESCRIPTION.txt");
        BApplication::setApplicationChangeLogFile(resource("changelog") + "/ChangeLog.txt");
        BApplication::setApplicationLicenseFile(resource("copying") + "/COPYING.txt");
        BApplication::setApplicationAuthorsFile(resource("infos/authors.beqt-info"));
        BApplication::setApplicationTranslationsFile(resource("infos/translators.beqt-info"));
        BApplication::setApplicationThanksToFile(resource("infos/thanks-to.beqt-info"));
        BApplication::aboutDialogInstance()->setupWithApplicationData();
        MainWindow *mw = new MainWindow;
        mw->show();
        BApplication::loadSettings();
        ret = app.exec();
        BApplication::saveSettings();
        delete mw;
#if defined(BUILTIN_RESOURCES)
        Q_CLEANUP_RESOURCE(cute_la2);
        Q_CLEANUP_RESOURCE(cute_la2_translations);
#endif
    }
    else
    {
        s.sendMessage(QCoreApplication::arguments());
    }
    return ret;
}
