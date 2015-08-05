#ifndef MANORSETTINGSTAB_H
#define MANORSETTINGSTAB_H

class BPasswordWidget;

class QCheckBox;
class QIcon;
class QLabel;
class QLineEdit;
class QPushButton;
class QSpinBox;
class QString;
class QTimeEdit;

#include <BAbstractSettingsTab>

#include <QObject>
#include <QStringList>
#include <QVariantMap>

/*============================================================================
================================ ManorSettingsTab ============================
============================================================================*/

class ManorSettingsTab : public BAbstractSettingsTab
{
    Q_OBJECT
private:
    QSpinBox *sboxOlympiadCheckInterval;
    QSpinBox *sboxTimerInterval;
    QSpinBox *sboxChatRowCount;
    QCheckBox *cboxStartAuto;
    QTimeEdit *tmedtStartAuto;
    QSpinBox *sboxTimeCorrection;
    QLabel *lblOlympiadMessageTemplate;
    QPushButton *btnGrabOlympiadMessage;
    QLabel *lblManorButtonPos;
    QPushButton *btnSelectManorButton;
public:
    explicit ManorSettingsTab();
public:
    QString id() const;
    QString title() const;
    QIcon icon() const;
    bool hasDefault() const;
    bool restoreDefault();
    bool saveSettings();
private slots:
    void grabOlympiadMessage();
    void selectManorButton();
private:
    Q_DISABLE_COPY(ManorSettingsTab)
};

#endif // MANORSETTINGSTAB_H
