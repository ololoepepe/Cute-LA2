#include "manorwidget.h"
#include "global.h"

#include <BTranslation>
#include <BApplication>
#include <BeQt>

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QFuture>
#include <QFutureWatcher>
#include <QCheckBox>
#include <QLabel>
#include <QFont>
#include <QPushButton>
#include <QTime>
#include <QtConcurrentMap>

class MyBool
{
public:
    MyBool() {bb = false;}
    MyBool(bool b) {bb = b;}
public:
    MyBool operatorOR(const MyBool &other) const {return bb || other.bb;}
public:
    bool operator=(const MyBool &other) {return bb = other.bb;}
    operator bool() {return bb;}
private:
    bool bb;
};

struct DetectParameters
{
    const QImage *src;
    int lineNumber;
    int width;
    const QImage *mask;
};

static MyBool detectOlympiadMessage(const DetectParameters &p)
{
    QImage img = p.src->copy(0, 15 * p.lineNumber, p.width, 11);
    QRgb rr = Global::getMainColor(img);
    img = Global::cutExtraSpace(Global::removeNoise(img, &rr), &rr);
    return img.createAlphaMask() == *p.mask;
}

static void reduceFunction(MyBool &finalResut, const MyBool &intermediateResult)
{
    finalResut = finalResut.operatorOR(intermediateResult);
}

ManorWidget::ManorWidget(QWidget *parent) :
    QWidget(parent)
{
    autoStartTimer.setInterval(BeQt::Minute);
    etimer.invalidate();
    connect(&autoStartTimer, SIGNAL(timeout()), this, SLOT(autoStartTimeout()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    //
    trActivate = BTranslation::translate("ManorWidget", "Activate", "btn text");
    trDeactivate = BTranslation::translate("ManorWidget", "Deactivate", "btn text");
    //
    QVBoxLayout *vlt = new QVBoxLayout(this);
      lbl = new QLabel;
        QFont fnt = lbl->font();
        fnt.setPointSize(40);
        lbl->setFont(fnt);
        lbl->setTextInteractionFlags(Qt::TextSelectableByMouse);
        lbl->setText(tr("Ready", "lbl text"));
      vlt->addWidget(lbl);
      QHBoxLayout *hlt = new QHBoxLayout;
        hlt->addStretch();
        cbox = new QCheckBox;
          cbox->setChecked(Global::manorAutoStartEnabled());
          connect(cbox, SIGNAL(toggled(bool)), this, SLOT(cboxToggled(bool)));
        hlt->addWidget(cbox);
        btn = new QPushButton;
          connect(btn, SIGNAL(clicked()), this, SLOT(btnClicked()));
        hlt->addWidget(btn);
      vlt->addLayout(hlt);
    //
    retranslateUi();
    connect(bApp, SIGNAL(languageChanged()), this, SLOT(retranslateUi()));
    instance = this;
}

void ManorWidget::resetTimerInterval()
{
    instance->timer.setInterval(instance->etimer.isValid() ? Global::manorTimerInterval() :
                                                             Global::olympiadCheckInterval());
}

void ManorWidget::resetAutoStartEnabled()
{
    instance->cbox->setChecked(Global::manorAutoStartEnabled());
}

void ManorWidget::resetAutoStart()
{
    if (Global::manorAutoStartEnabled())
        instance->autoStartTimeout();
}

void ManorWidget::retranslateUi()
{
    cbox->setText(tr("Activate automatically", "cbox text"));
    btn->setText(timer.isActive() ? trDeactivate : trActivate);
}

void ManorWidget::autoStartTimeout()
{
    if (btn->isEnabled() && !timer.isActive() && Global::manorAutoStartEnabled()
            && QTime::currentTime() >= Global::manorAutoStartTime())
        btn->animateClick();
}

void ManorWidget::timeout()
{
    if (!etimer.isValid())
    {
        QPoint pos = Global::chatBottomPos() + QPoint(0, -12) + QPoint(0, -15 * (Global::chatRowCount() - 1));
        int w = Global::olympiadMessageMask()->width();
        QImage s = Global::grabDesktop(pos, w, 15 * (Global::chatRowCount() - 1) + 12);
        DetectParameters p;
        p.src = &s;
        p.width = w;
        p.mask = Global::olympiadMessageMask();
        QList<DetectParameters> list;
        foreach (int i, bRangeD(0, Global::chatRowCount()))
        {
            p.lineNumber = i;
            list << p;
        }
        QFuture<MyBool> f = QtConcurrent::mappedReduced(list, &detectOlympiadMessage, &reduceFunction);
        QFutureWatcher<MyBool> fw;
        fw.setFuture(f);
        BeQt::waitNonBlocking(&fw, SIGNAL(finished()));
        if (f.result())
        {
            if (cbox->isChecked())
                cbox->setChecked(false);
            btn->setEnabled(false);
            msecs = 6 * BeQt::Minute + Global::manorTimeCorrection() - 10;
            //10 ms is the delay between moving the cursor and clicking
            etimer.start();
            timer.setInterval(Global::manorTimerInterval());
            timeout();
        }
    }
    else
    {
        qint64 r = msecs - etimer.elapsed();
        if (r > 0)
        {
            QString m = QString::number(r / (BeQt::Minute));
            if (m.length() < 2)
                m.prepend("0");
            r = r % (BeQt::Minute);
            QString s = QString::number(r / BeQt::Second);
            if (s.length() < 2)
                s.prepend("0");
            QString ms = QString::number(r % BeQt::Second);
            ms.prepend(QString().fill('0', 3 - ms.length()));
            lbl->setText(m + ":" + s + ":" + ms);
        }
        else
        {
            Global::emulateMouseClick(Qt::LeftButton, Global::manorButtonPos());
            timer.stop();
            etimer.invalidate();
            lbl->setText(tr("Sold!", "lbl text"));
            btn->setEnabled(true);
            btn->setText(trActivate);
        }
    }
}

void ManorWidget::cboxToggled(bool b)
{
    Global::setManorAutoStartEnabled(b);
    if (b)
    {
        autoStartTimeout();
        autoStartTimer.start(BeQt::Minute);
    }
    else
    {
        autoStartTimer.stop();
    }
    Global::switchToWindow();
}

void ManorWidget::btnClicked()
{
    if (timer.isActive())
    {
        timer.stop();
        lbl->setText(tr("Ready", "lbl text"));
    }
    else
    {
        lbl->setText(tr("Activated", "lbl text"));
        timer.start();
    }
    btn->setText(timer.isActive() ? trDeactivate : trActivate);
    Global::switchToWindow();
}

ManorWidget *ManorWidget::instance = 0;
