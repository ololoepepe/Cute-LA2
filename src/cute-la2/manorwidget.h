#ifndef MANORWIDGET_H
#define MANORWIDGET_H

class QCheckBox;
class QLabel;
class QPushButton;
class QTabWidget;

#include <BTranslation>

#include <QTimer>
#include <QElapsedTimer>
#include <QWidget>

class ManorWidget : public QWidget
{
    Q_OBJECT
private:
    static ManorWidget *instance;
private:
    QLabel *lbl;
    QCheckBox *cbox;
    QPushButton *btn;
    QTimer timer;
    QTimer autoStartTimer;
    QElapsedTimer etimer;
    qint64 msecs;
    BTranslation trActivate;
    BTranslation trDeactivate;
public:
    explicit ManorWidget(QWidget *parent = 0);
public:
    static void resetTimerInterval();
    static void resetAutoStartEnabled();
    static void resetAutoStart();
private slots:
    void retranslateUi();
    void timeout();
    void autoStartTimeout();
    void cboxToggled(bool b);
    void btnClicked();
};

#endif // MANORWIDGET_H
