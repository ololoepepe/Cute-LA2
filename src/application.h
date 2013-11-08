#ifndef APPLICATION_H
#define APPLICATION_H

class BAbstractSettingsTab;

class QWidget;

#include <BApplication>
#include <BSettingsDialog>

#include <QObject>
#include <QMap>
#include <QFont>
#include <QTextCodec>
#include <QStringList>
#include <QByteArray>
#include <QLocale>
#include <QList>

#if defined(bApp)
#undef bApp
#endif
#define bApp (static_cast<Application *>(BApplication::instance()))

/*============================================================================
================================ Application =================================
============================================================================*/

class Application : public BApplication
{
    Q_OBJECT
public:
    explicit Application();
    ~Application();
//public:
    //static void handleExternalRequest(const QStringList &args);
    //static bool showPasswordDialog(QWidget *parent = 0);
    //static bool showRegisterDialog(QWidget *parent = 0);
    //static bool showSettings(QWidget *parent = 0);
protected:
    QList<BAbstractSettingsTab *> createSettingsTabs() const;
//private:
    //static bool testAppInit();
//private:
    //void addMainWindow(const QStringList &fileNames = QStringList());
//private slots:
    //void mainWindowDestroyed(QObject *obj);
//private:
    //bool minitialWindowCreated;
    //QMap<QObject *, MainWindow *> mmainWindows;
private:
    Q_DISABLE_COPY(Application)
};

#endif // APPLICATION_H
