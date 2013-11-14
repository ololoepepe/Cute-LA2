#ifndef TIMERWIDGET_H
#define TIMERWIDGET_H

class QLabel;
class QPushButton;

#include <BTranslation>

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>

class TimerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimerWidget(QWidget *parent = 0);
public:
    static void resetTimerInterval();
private slots:
    void retranslateUi();
    void timeout();
    void btnStartClicked();
    void btnPauseClicked();
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
};

#endif // TIMERWIDGET_H
