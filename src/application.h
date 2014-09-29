#ifndef APPLICATION_H
#define APPLICATION_H

class MainWindow;

class BAbstractSettingsTab;

class QString;

#include <BApplication>

#include <QList>
#include <QObject>

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
public:
    explicit Application(int &argc, char **argv, const QString &applicationName, const QString &organizationName);
    ~Application();
protected:
    QList<BAbstractSettingsTab *> createSettingsTabs() const;
private:
    Q_DISABLE_COPY(Application)
};

#endif // APPLICATION_H
