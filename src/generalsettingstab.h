#ifndef GENERALSETTINGSTAB_H
#define GENERALSETTINGSTAB_H

class QCheckBox;
class QIcon;
class QLineEdit;
class QSpinBox;
class QString;

class BLocaleComboBox;

#include <BAbstractSettingsTab>

#include <QObject>

/*============================================================================
================================ GeneralSettingsTab ==========================
============================================================================*/

class GeneralSettingsTab : public BAbstractSettingsTab
{
    Q_OBJECT
private:
    BLocaleComboBox *mlcmbox;
    QLineEdit *ledtGameDir;
    QSpinBox *sboxTimerInterval;
    QSpinBox *sboxDelay;
    QCheckBox *cboxDetectWindow;
public:
    explicit GeneralSettingsTab();
public:
    QString id() const;
    QString title() const;
    QIcon icon() const;
    bool hasDefault() const;
    bool restoreDefault();
    bool saveSettings();
private slots:
    void selectGameDir();
private:
    Q_DISABLE_COPY(GeneralSettingsTab)
};

#endif // GENERALSETTINGSTAB_H
