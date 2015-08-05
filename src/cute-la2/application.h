#ifndef APPLICATION_H
#define APPLICATION_H

class MainWindow;

class BAbstractSettingsTab;

class QAction;
class QMenu;
class QString;

#include <BApplication>

#include <QList>
#include <QObject>
#include <QSystemTrayIcon>

#if defined(bApp)
#   undef bApp
#endif
#define bApp (static_cast<Application *>(BApplication::instance()))

/*============================================================================
================================ Application =================================
============================================================================*/

class Application : public BApplication
{
    Q_OBJECT
private:
    MainWindow *mmainWindow;
    QSystemTrayIcon *mtray;
    QMenu *mmnu;
    QAction *mactShowHide;
    QAction *mactQuit;
public:
    explicit Application(int &argc, char **argv, const QString &applicationName, const QString &organizationName);
    ~Application();
public:
    static QAction *actionQuit();
protected:
    QList<BAbstractSettingsTab *> createSettingsTabs() const;
private slots:
    void retranslateUi();
    void actShowHideTriggered();
    void actQuitTriggered();
    void trayActivated(QSystemTrayIcon::ActivationReason reason);
private:
    Q_DISABLE_COPY(Application)
};

#endif // APPLICATION_H
