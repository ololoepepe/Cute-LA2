#ifndef APPLICATION_H
#define APPLICATION_H

class BAbstractSettingsTab;

#include <BApplication>

#include <QObject>
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
protected:
    QList<BAbstractSettingsTab *> createSettingsTabs() const;
private:
    Q_DISABLE_COPY(Application)
};

#endif // APPLICATION_H
