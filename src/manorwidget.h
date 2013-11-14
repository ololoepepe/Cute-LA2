#ifndef MANORWIDGET_H
#define MANORWIDGET_H

#include <BTranslation>

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>

class QTabWidget;
class QLabel;
class QCheckBox;
class QPushButton;

class ManorWidget : public QWidget
{
    Q_OBJECT
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
};

#endif // MANORWIDGET_H
