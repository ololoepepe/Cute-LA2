#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

class QLabel;
class QPushButton;

#include <BTranslation>

#include <QElapsedTimer>
#include <QTimer>
#include <QWidget>

class TimerWidget : public QWidget
{
    Q_OBJECT
private:
    static TimerWidget *instance;
private:
    QLabel *lbl;
    QPushButton *btnStart;
    QPushButton *btnPause;
    QTimer timer;
    QElapsedTimer etimer;
    qint64 msecs;
    BTranslation trPause;
    BTranslation trUnpause;
public:
    explicit TimerWidget(QWidget *parent = 0);
public:
    static void resetTimerInterval();
private slots:
    void retranslateUi();
    void timeout();
    void btnStartClicked();
    void btnPauseClicked();
};

#endif // TIMERWIDGET_H
