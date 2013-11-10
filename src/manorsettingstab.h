#ifndef MANORSETTINGSTAB_H
#define MANORSETTINGSTAB_H

class BPasswordWidget;

class QString;
class QIcon;
class QLineEdit;
class QSpinBox;
class QPushButton;
class QLabel;
class QCheckBox;
class QTimeEdit;

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
    void selectManorButton();
private:
    QSpinBox *sboxOlympiadCheckInterval;
    QSpinBox *sboxTimerInterval;
    QSpinBox *sboxChatRowCount;
    QCheckBox *cboxStartAuto;
    QTimeEdit *tmedtStartAuto;
    QLabel *lblOlympiadMessageTemplate;
    QPushButton *btnGrabOlympiadMessage;
    QLabel *lblManorButtonPos;
    QPushButton *btnSelectManorButton;
private:
    Q_DISABLE_COPY(ManorSettingsTab)
};

#endif // MANORSETTINGSTAB_H
