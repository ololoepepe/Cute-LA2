#ifndef FISHINGSETTINGSTAB_H
#define FISHINGSETTINGSTAB_H

class QString;
class QIcon;
class QComboBox;
class QSpinBox;
class QCheckBox;

#include <BAbstractSettingsTab>

#include <QObject>
#include <QList>

class FishingSettingsTab : public BAbstractSettingsTab
{
    Q_OBJECT
public:
    explicit FishingSettingsTab();
public:
    QString title() const;
    QIcon icon() const;
    bool hasDefault() const;
    bool restoreDefault();
    bool saveSettings();
private slots:
    void cmboxCurrentIndexChanged(int index);
private:
    QCheckBox *cboxEquip;
    QSpinBox *sboxDelay;
    QComboBox *cmboxPanel;
    QList<QComboBox *> cmboxes;
};

#endif // FISHINGSETTINGSTAB_H
