#ifndef CRAFTWIDGET_H
#define CRAFTWIDGET_H

class QSpinBox;
class QPushButton;
class QLabel;
class QHBoxLayout;

#include <BTranslation>

#include <QWidget>
#include <QTimer>

class CraftWidget : public QWidget
{
    Q_OBJECT
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
    BTranslation trStart;
    BTranslation trStop;
};

#endif // CRAFTWIDGET_H
