#ifndef FISHINGSETTINGSTAB_H
#define FISHINGSETTINGSTAB_H

class QCheckBox;
class QComboBox;
class QIcon;
class QSpinBox;
class QString;

#include <BAbstractSettingsTab>

#include <QList>
#include <QObject>

class FishingSettingsTab : public BAbstractSettingsTab
{
    Q_OBJECT
private:
    QCheckBox *cboxEquip;
    QSpinBox *sboxDelay;
    QComboBox *cmboxPanel;
    QComboBox *cmboxMainPanel;
    QSpinBox *sboxRestTime;
    QList<QComboBox *> cmboxes;
public:
    explicit FishingSettingsTab();
public:
    QString id() const;
    QString title() const;
    QIcon icon() const;
    bool hasDefault() const;
    bool restoreDefault();
    bool saveSettings();
private slots:
    void cmboxCurrentIndexChanged(int index);
};

#endif // FISHINGSETTINGSTAB_H
