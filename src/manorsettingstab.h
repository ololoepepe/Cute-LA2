#ifndef MANORSETTINGSTAB_H
#define MANORSETTINGSTAB_H

class BPasswordWidget;

class QString;
class QIcon;
class QLineEdit;
class QSpinBox;
class QPushButton;

#include <BAbstractSettingsTab>

#include <QObject>
#include <QVariantMap>
#include <QStringList>

/*============================================================================
================================ ManorSettingsTab ============================
============================================================================*/

class ManorSettingsTab : public BAbstractSettingsTab
{
    Q_OBJECT
public:
    explicit ManorSettingsTab();
public:
    QString title() const;
    QIcon icon() const;
    bool hasDefault() const;
    bool restoreDefault();
    bool saveSettings();
private slots:
    void grabOlympiadMessage();
private:
    QSpinBox *sboxOlympiadCheckInterval;
    QSpinBox *sboxTimerInterval;
    QSpinBox *sboxChatRowCount;
    QPushButton *btnGrablympiadMessage;
private:
    Q_DISABLE_COPY(ManorSettingsTab)
};

#endif // MANORSETTINGSTAB_H
