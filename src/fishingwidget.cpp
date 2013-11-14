#include "fishingwidget.h"
#include "global.h"

#include <BTranslation>
#include <BDirTools>
#include <BApplication>

#include <QWidget>
#include <QImage>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QString>
#include <QKeySequence>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPoint>
#include <QTimer>

enum BasicColor
{
    Red = 1,
    Green = 2,
    Blue = 4
};

static int colorWeight(const QImage &img, int color)
{
    int weight = 0;
    for (int x = 0; x < img.width(); ++x)
    {
        for (int y = 0; y < img.height(); ++y)
        {
            if (Red & color)
                weight += qRed(img.pixel(x, y));
            if (Green & color)
                weight += qGreen(img.pixel(x, y));
            if (Blue & color)
                weight += qBlue(img.pixel(x, y));
        }
    }
    return weight;
}

static void waitMsecs(int msecs, QEventLoop *&el)
{
    if (msecs <= 0)
        return;
    QEventLoop l;
    el = &l;
    QTimer::singleShot(msecs, &l, SLOT(quit()));
    l.exec();
}

FishingWidget::FishingWidget(QWidget *parent) :
    QWidget(parent)
{
    loop = 0;
    mustExit = false;
    QString fn = BDirTools::findResource("pixmaps/target_close.png", BDirTools::GlobalOnly);
    targetClose = QImage(fn).convertToFormat(QImage::Format_RGB32);
    active = false;
    fishing = false;
    trStart = BTranslation::translate("FishingWidget", "Start fishing", "btn text");
    trStop = BTranslation::translate("FishingWidget", "Stop fishing", "btn text");
    //
    QVBoxLayout *vlt = new QVBoxLayout(this);
      QHBoxLayout *hlt = new QHBoxLayout;
        btn = new QPushButton;
          connect(btn, SIGNAL(clicked()), this, SLOT(btnClicked()));
        hlt->addWidget(btn);
      vlt->addLayout(hlt);
      ptedt = new QPlainTextEdit;
        ptedt->setReadOnly(true);
      vlt->addWidget(ptedt);
    //
    retranslateUi();
    connect(bApp, SIGNAL(languageChanged()), this, SLOT(retranslateUi()));
    instance = this;
}

void FishingWidget::quitLoop()
{
    instance->mustExit = true;
    if (instance->loop)
        instance->loop->exit();
}

bool FishingWidget::testPecked(bool anyHp)
{
    QImage s = Global::grabDesktop(Global::fishHpPos(), 1, 11);
    if (colorWeight(s, Blue) >= 1400)
        return true;
    if (anyHp)
    {
        if (colorWeight(s, Red) < 600)
            return false;
        s = Global::grabDesktop(Global::fishHpPos() + QPoint(150, -232), 1, 12);
        return colorWeight(s, Blue) >= 1400;
    }
    return false;
}

bool FishingWidget::testTarget()
{
    QImage s = Global::grabDesktop(Global::targetClosePos(), 14, 14);
    return s == targetClose;
}

int FishingWidget::getFishHp()
{
    QImage s = Global::grabDesktop(Global::fishHpPos(), 230, 11);
    for (int i = 0; i < 230; ++i)
        if (colorWeight(s.copy(i, 0, 1, 11), Red) >= 600)
            return i;
    return 230;
}

void FishingWidget::useReeling()
{
    Global::emulateKeyPress(Global::fishingKey(Global::UseFishingShot));
    waitMsecs(200, loop);
    if (mustExit)
        return;
    else
        loop = 0;
    Global::emulateKeyPress(Global::fishingKey(Global::UseReeling));
}

void FishingWidget::usePumping()
{
    Global::emulateKeyPress(Global::fishingKey(Global::UseFishingShot));
    waitMsecs(200, loop);
    if (mustExit)
        return;
    else
        loop = 0;
    Global::emulateKeyPress(Global::fishingKey(Global::UsePumping));
}

void FishingWidget::logFishing(const QString &text)
{
    if (Qt::mightBeRichText(text))
        ptedt->appendHtml(text);
    else
        ptedt->appendPlainText(text);
}

bool FishingWidget::wait(int msecs)
{
    waitMsecs(msecs, loop);
    if (mustExit)
        fishing = false;
    else
        loop = 0;
    return !mustExit;
}

void FishingWidget::retranslateUi()
{
    btn->setText(active ? trStop : trStart);
}

void FishingWidget::btnClicked()
{
    active = !active;
    btn->setText(active ? trStop : trStart);
    if (!active || fishing)
        return Global::switchToWindow();
    fishing = true;
    int delay = Global::fishingStartDelay();
    logFishing(tr("Preparing to fish. Waiting for") + " " + QString::number(delay) + " " + tr("seconds..."));
    Global::switchToWindow();
    waitMsecs(delay * BeQt::Second, loop);
    if (mustExit)
    {
        fishing = false;
        return;
    }
    else
        loop = 0;
    if (!active)
    {
        fishing = false;
        return logFishing(tr("Fishing cancelled."));
    }
    logFishing(tr("Switching to fishing panel..."));
    Global::emulateKeyPress("Alt+F" + QString::number(Global::fishingPanelNumber()));
    if (!wait(1000))
        return;
    if (Global::fishingEquipBeforeStart())
    {
        logFishing(tr("Equipping fishing gear..."));
        Global::emulateKeyPress(Global::fishingKey(Global::EquipRod));
        if (!wait(1000))
            return;
        Global::emulateKeyPress(Global::fishingKey(Global::EquipBait));
        if (!wait(1000))
            return;
    }
    while (active)
    {
        logFishing("<font color=blue>" + tr("Starting fishing...") + "</font>");
        Global::emulateKeyPress(Global::fishingKey(Global::UseFishing));
        volatile bool pecked = false;
        logFishing(tr("Waiting for a fish to peck..."));
        if (!wait(5000))
            return;
        for (int i = 0; i < 150; ++i)
        {
            if (!wait(100))
                return;
            if (testPecked(false))
            {
                logFishing("<font color=blue>" + tr("A fish pecked!") + "</font>");
                pecked = true;
                break;
            }
        }
        if (!pecked)
        {
            logFishing("<font color=red>" + tr("No fish pecked.") + "</font> " + tr("Waiting a bit..."));
            if (!wait(2000))
                return;
            logFishing("<font color=green>" + tr("Cycle finished.") + "</font>");
            continue;
        }
        if (!wait(1000))
            return;
        while (testPecked())
        {
            logFishing(tr("Deciding which skill to use..."));
            int prevHP = getFishHp();
            if (!wait(500))
                return;
            if (!testPecked())
            {
                logFishing(tr("No need to use skills."));
                break;
            }
            if (getFishHp() > prevHP)
            {
                logFishing("<font color=magenta>" + tr("Using Reeling...") + "</font>");
                useReeling();
                if (!wait(1500))
                    return;
            }
            else
            {
                prevHP = getFishHp();
                if (!wait(500))
                    return;
                if (!testPecked())
                {
                    logFishing(tr("No need to use skills."));
                    break;
                }
                if (getFishHp() > prevHP)
                {
                    logFishing("<font color=magenta>" + tr("Using Reeling...") + "</font>");
                    useReeling();
                    if (!wait(1500))
                        return;
                }
                else
                {
                    if (testPecked(false))
                    {
                        logFishing("<font color=orange>" + tr("Using Pumping...") + "</font>");
                        usePumping();
                    }
                    else
                    {
                        logFishing(tr("No need to use skills."));
                    }
                    if (!wait(1000))
                        return;
                }
            }
        }
        logFishing("<font color=green>" + tr("Fishing finished!") + "</font> "
                   + tr("Waiting for a possible attack..."));
        if (!wait(2000))
            return;
        if (testTarget())
        {
            logFishing("<font color=blue>" + tr("A monster attacks!") + "</font> "
                       + tr("Equipping arms and counterattacking..."));
            Global::emulateKeyPress(Global::fishingKey(Global::EquipWeapon) + ","
                                    + Global::fishingKey(Global::Attack));
            while (testTarget())
                if (!wait(100))
                    return;
            logFishing("<font color=blue>" + tr("The monster is killed!") + "</font> "
                       + tr("Equipping fishing gear..."));
            //TODO: While collecting dropped items, a character may turn away from the water.
            Global::emulateKeyPress(Global::fishingKey(Global::EquipRod));
            if (!wait(1000))
                return;
            Global::emulateKeyPress(Global::fishingKey(Global::EquipBait));
            if (!wait(1000))
                return;
            //The following is needed to make fishing possible again (LA2 bug?)
            Global::emulateKeyPress(Global::fishingKey(Global::SitStand));
            if (!wait(4000))
                return;
            Global::emulateKeyPress(Global::fishingKey(Global::SitStand));
            if (!wait(4000))
                return;
        }
        else
        {
            logFishing(tr("No monster. Waiting a bit..."));
        }
        if (!wait(2000))
            return;
        logFishing("<font color=green>" + tr("Cycle finished.") + "</font>");
    }
    logFishing(tr("Switching to main panel..."));
    Global::emulateKeyPress("Alt+F" + QString::number(Global::mainPanelNumber()));
    logFishing(tr("Stopped fishing."));
    fishing = false;
}

FishingWidget *FishingWidget::instance = 0;
