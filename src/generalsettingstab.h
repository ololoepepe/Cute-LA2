#ifndef GENERALSETTINGSTAB_H
#define GENERALSETTINGSTAB_H

class QString;
class QIcon;
class QLineEdit;
class QSpinBox;

class BLocaleComboBox;

#include <BAbstractSettingsTab>

#include <QObject>

/*============================================================================
================================ GeneralSettingsTab ==========================
============================================================================*/

class GeneralSettingsTab : public BAbstractSettingsTab
{
    Q_OBJECT
public:
    explicit GeneralSettingsTab();
public:
    QString title() const;
    QIcon icon() const;
    bool hasDefault() const;
    bool restoreDefault();
    bool saveSettings();
private slots:
    void selectGameDir();
private:
    BLocaleComboBox *mlcmbox;
    QLineEdit *ledtGameDir;
    QSpinBox *sboxTimerInterval;
    QSpinBox *sboxDelay;
private:
    Q_DISABLE_COPY(GeneralSettingsTab)
};

#endif // GENERALSETTINGSTAB_H
