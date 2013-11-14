#ifndef CRAFTWIDGET_H
#define CRAFTWIDGET_H

class QSpinBox;
class QPushButton;

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
private:
    QPushButton *btn;
    QSpinBox *sboxRegen;
    QSpinBox *sboxConsumption;
    QTimer timer;
    BTranslation trStart;
    BTranslation trStop;
};

#endif // CRAFTWIDGET_H
