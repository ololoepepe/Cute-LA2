#ifndef CRAFTWIDGET_H
#define CRAFTWIDGET_H

class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpinBox;

#include <BTranslation>

#include <QTimer>
#include <QWidget>

class CraftWidget : public QWidget
{
    Q_OBJECT
private:
    QPushButton *btn;
    QSpinBox *sboxCount;
    QSpinBox *sboxDelay;
    QSpinBox *sboxRegen;
    QSpinBox *sboxConsumption;
    QHBoxLayout *hltButton;
    QLabel *lblButtonPos;
    QPushButton *btnSelectButton;
    QTimer timer;
    bool active;
    BTranslation trStart;
    BTranslation trStop;
public:
    explicit CraftWidget(QWidget *parent = 0);
private:
    int calculateTimeout();
private slots:
    void retranslateUi();
    void timeout();
    void btnClicked();
    void selectButton();
    void sboxDelayValueChanged(int value);
    void sboxRegenValueChanged(int value);
    void sboxConsumptionValueChanged(int value);
};

#endif // CRAFTWIDGET_H
