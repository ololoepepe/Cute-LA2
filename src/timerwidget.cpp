#include "timerwidget.h"
#include "global.h"

#include <BApplication>
#include <BeQt>
#include <BTranslation>

#include <QString>
#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QFont>

#include <QDebug>

TimerWidget::TimerWidget(QWidget *parent) :
    QWidget(parent)
{
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    trPause = BTranslation::translate("TimerWidget", "Pause", "btn text");
    trUnpause = BTranslation::translate("TimerWidget", "Unpause", "btn text");
    //
    QVBoxLayout *vlt = new QVBoxLayout(this);
      lbl = new QLabel;
        QFont fnt = lbl->font();
        fnt.setPointSize(40);
        lbl->setFont(fnt);
        lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);
        lbl->setText("00:00:00:000");
      vlt->addWidget(lbl);
      QHBoxLayout *hlt = new QHBoxLayout;
        btnStart = new QPushButton;
          connect(btnStart, SIGNAL(clicked()), this, SLOT(btnStartClicked()));
        hlt->addWidget(btnStart);
        btnPause = new QPushButton;
          btnPause->setEnabled(false);
          connect(btnPause, SIGNAL(clicked()), this, SLOT(btnPauseClicked()));
        hlt->addWidget(btnPause);
      vlt->addLayout(hlt);
    //
    retranslateUi();
    connect(bApp, SIGNAL(languageChanged()), this, SLOT(retranslateUi()));
    instance = this;
}

void TimerWidget::resetTimerInterval()
{
    if (instance->timer.isActive())
        instance->timeout();
    instance->timer.setInterval(Global::timerInterval());
}

void TimerWidget::retranslateUi()
{
    btnStart->setText(tr("Start", "btn text"));
    btnPause->setText(timer.isActive() ? trPause : trUnpause);
}

void TimerWidget::timeout()
{
    qint64 r = msecs + etimer.elapsed();
    QString h = QString::number(r / (BeQt::Hour));
    if (h.length() < 2)
        h.prepend("0");
    r = r % (BeQt::Hour);
    QString m = QString::number(r / (BeQt::Minute));
    if (m.length() < 2)
        m.prepend("0");
    r = r % (BeQt::Minute);
    QString s = QString::number(r / BeQt::Second);
    if (s.length() < 2)
        s.prepend("0");
    QString ms = QString::number(r % BeQt::Second);
    ms.prepend(QString().fill('0', 3 - ms.length()));
    lbl->setText(h + ":" + m + ":" + s + ":" + ms);
}

void TimerWidget::btnStartClicked()
{
    etimer.invalidate();
    etimer.start();
    timer.stop();
    btnPause->setEnabled(true);
    btnPause->setText(trPause);
    msecs = 0;
    timer.start(Global::timerInterval());
}

void TimerWidget::btnPauseClicked()
{
    if (timer.isActive())
    {
        timer.stop();
        msecs += etimer.elapsed();
        etimer.invalidate();
        btnPause->setText(trUnpause);
    }
    else
    {
        timer.start(Global::timerInterval());
        etimer.invalidate();
        etimer.start();
        btnPause->setText(trPause);
    }
}

TimerWidget *TimerWidget::instance = 0;
