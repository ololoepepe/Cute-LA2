#ifndef CALC_H
#define CALC_H

class CalcWidget;

class BAboutDialog;
class BAbstractSettingsTab;
class BVersion;

class QAction;
class QPixmap;
class QStringList;

#include <BGuiPluginInterface>
#include <BPluginInterface>

#include <QList>
#include <QObject>
#include <QString>
#include <QtPlugin>

/*============================================================================
================================ Calc ========================================
============================================================================*/

class Calc : public QObject, public BPluginInterface, public BGuiPluginInterface
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    Q_PLUGIN_METADATA(IID "Cute-LA2.Calc")
#endif
    Q_OBJECT
    Q_INTERFACES(BPluginInterface)
    Q_INTERFACES(BGuiPluginInterface)
private:
    QAction *mact;
    QAction *msep;
    CalcWidget *mwgt;
public:
    explicit Calc();
    ~Calc();
public:
    void activate();
    BAboutDialog *createAboutDialog();
    QList<BAbstractSettingsTab *> createSettingsTabs();
    void deactivate();
    QString helpIndex() const;
    QStringList helpSearchPaths() const;
    QString id() const;
    PluginInfo info() const;
    QPixmap pixmap() const;
    bool prefereStaticInfo() const;
    void processStandardAboutDialog(BAboutDialog *dlg) const;
    StaticPluginInfo staticInfo() const;
    QString title() const;
    QString type() const;
    BVersion version() const;
private slots:
    void showCalc();
};

#endif // CALC_H
