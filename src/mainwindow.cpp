#include "mainwindow.h"
#include "global.h"

#include <BApplication>
#include <BTranslation>
#include <BDirTools>

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QTimer>
#include <QFont>
#include <QElapsedTimer>
#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include <QImage>
#include <QPlainTextEdit>
#include <QMenu>
#include <QAction>
#include <QKeySequence>
#include <QMenuBar>
#include <QTabWidget>
#include <QSplitter>
#include <QPoint>
#include <QSettings>
#include <QCloseEvent>
#include <QMessageBox>

#include <QDebug>

#if defined(Q_OS_WIN)
#include "windows.h"
#endif

static void waitMsecs(int msecs, QEventLoop *&el)
{
    if (msecs <= 0)
        return;
    QEventLoop l;
    el = &l;
    QTimer::singleShot(msecs, &l, SLOT(quit()));
    l.exec();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QString fn = BDirTools::findResource("pixmaps/target_close.png", BDirTools::GlobalOnly);
    targetClose = QImage(fn).convertToFormat(QImage::Format_RGB32);
    fn = BDirTools::findResource("pixmaps/fish_hp.png", BDirTools::GlobalOnly);
    fishHp = QImage(fn).convertToFormat(QImage::Format_RGB32);
    fn = BDirTools::findResource("pixmaps/fish_hp_background.png", BDirTools::GlobalOnly);
    fishHpBackground = QImage(fn).convertToFormat(QImage::Format_RGB32);
    fishingActive = false;
    fishing = false;
    loop = 0;
    mustExit = false;
    manorEtimer.invalidate();
    windowPos = QPoint(-1, -1);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerTimeout()));
    connect(&manorTimer, SIGNAL(timeout()), this, SLOT(manorTimerTimeout()));
    //
    trManorTurnOn = BTranslation::translate("MainWindow", "Turn on", "btn text");
    trManorTurnOff = BTranslation::translate("MainWindow", "Turn off", "btn text");
    trTimerPause = BTranslation::translate("MainWindow", "Pause", "btn text");
    trTimerUnpause = BTranslation::translate("MainWindow", "Unpause", "btn text");
    trFishingStart = BTranslation::translate("MainWindow", "Start fishing", "btn text");
    trFishingStop = BTranslation::translate("MainWindow", "Stop fishing", "btn text");
    //
    spltr = new QSplitter(Qt::Vertical);
    spltr->setObjectName("splitter");
      QWidget *wgt = new QWidget;
        QVBoxLayout *vlt = new QVBoxLayout(wgt);
          twgtManor = new QTabWidget;
            twgtManor->setTabPosition(QTabWidget::West);
            wgtAutoManor = new QWidget;
              wgtAutoManor->setEnabled(false);
              QVBoxLayout *vltam = new QVBoxLayout(wgtAutoManor);
                lblAutoManor = new QLabel;
                  QFont fnt = lblAutoManor->font();
                  fnt.setPointSize(40);
                  lblAutoManor->setFont(fnt);
                  lblAutoManor->setTextInteractionFlags(Qt::TextSelectableByMouse);
                  lblAutoManor->setText("Press Start");
                vltam->addWidget(lblAutoManor);
                QHBoxLayout *hltam = new QHBoxLayout;
                  hltam->addStretch();
                  btnManor = new QPushButton;
                    connect(btnManor, SIGNAL(clicked()), this, SLOT(btnManorClicked()));
                  hltam->addWidget(btnManor);
                vltam->addLayout(hltam);
            twgtManor->addTab(wgtAutoManor, "");
            QWidget *wgtt = new QWidget;
              QVBoxLayout *vltt = new QVBoxLayout(wgtt);
                lblTimer = new QLabel;
                  lblTimer->setFont(fnt);
                  lblTimer->setTextInteractionFlags(Qt::TextSelectableByMouse);
                  lblTimer->setText("00:00:00:000");
                vltt->addWidget(lblTimer);
                QHBoxLayout *hltt = new QHBoxLayout;
                  btnTimerStart = new QPushButton;
                    connect(btnTimerStart, SIGNAL(clicked()), this, SLOT(btnTimerStartClicked()));
                  hltt->addWidget(btnTimerStart);
                  btnTimerPause = new QPushButton;
                    btnTimerPause->setEnabled(false);
                    connect(btnTimerPause, SIGNAL(clicked()), this, SLOT(btnTimerPauseClicked()));
                  hltt->addWidget(btnTimerPause);
                vltt->addLayout(hltt);
            twgtManor->addTab(wgtt, "");
          vlt->addWidget(twgtManor);
        spltr->addWidget(wgt);
        wgt = new QWidget;
          vlt = new QVBoxLayout(wgt);
            lblInfo = new QLabel;
            vlt->addWidget(lblInfo);
            QHBoxLayout *hlt = new QHBoxLayout;
              cboxOnTop = new QCheckBox;
                connect(cboxOnTop, SIGNAL(toggled(bool)), this, SLOT(cboxToggled(bool)));
              hlt->addWidget(cboxOnTop);
              btnDetect = new QPushButton;
                connect(btnDetect, SIGNAL(clicked()), this, SLOT(btnDetectClicked()));
              hlt->addWidget(btnDetect);
            vlt->addLayout(hlt);
        spltr->addWidget(wgt);
      twgtBots = new QTabWidget;
        wgtFishing = new QWidget;
          wgtFishing->setEnabled(false);
          QVBoxLayout *vltf = new QVBoxLayout(wgtFishing);
            QHBoxLayout *hltf = new QHBoxLayout;
              btnFishing = new QPushButton;
                connect(btnFishing, SIGNAL(clicked()), this, SLOT(btnFishingClicked()));
              hltf->addWidget(btnFishing);
            vltf->addLayout(hltf);
            ptedtFishing = new QPlainTextEdit;
              ptedtFishing->setReadOnly(true);
            vltf->addWidget(ptedtFishing);
        twgtBots->addTab(wgtFishing, "");
      spltr->addWidget(twgtBots);
    setCentralWidget(spltr);
    //File
    mmnuFile = menuBar()->addMenu("");
    mactQuit = mmnuFile->addAction("");
    mactQuit->setMenuRole(QAction::QuitRole);
    mactQuit->setIcon(BApplication::icon("exit"));
    mactQuit->setShortcut(QKeySequence("Ctrl+Q"));
    connect(mactQuit, SIGNAL(triggered()), this, SLOT(close()));
    //Edit
    mmnuEdit = menuBar()->addMenu("");
    QAction *act = BApplication::createStandardAction(BApplication::SettingsAction);
    act->setShortcut(QKeySequence("Ctrl+P"));
    mmnuEdit->addAction(act);
    //Help
    mmnuHelp = menuBar()->addMenu("");
    mmnuHelp->addAction( BApplication::createStandardAction(BApplication::HomepageAction) );
    mmnuHelp->addSeparator();
    act = BApplication::createStandardAction(BApplication::HelpContentsAction);
    act->setShortcut(QKeySequence("F1"));
    mmnuHelp->addAction(act);
    mmnuHelp->addAction(BApplication::createStandardAction(BApplication::WhatsThisAction));
    mmnuHelp->addSeparator();
    mmnuHelp->addAction(BApplication::createStandardAction(BApplication::AboutAction));
    //
    retranslateUi();
    connect(bApp, SIGNAL(languageChanged()), this, SLOT(retranslateUi()));
    restoreGeometry(bSettings->value("GUI/main_window_geometry").toByteArray());
    restoreState(bSettings->value("GUI/main_window_state").toByteArray());
    spltr->restoreState(bSettings->value("GUI/splitter_state").toByteArray());
    cboxOnTop->setChecked(bSettings->value("GUI/stay_on_top").toBool());
    inst = this;
    reloadInfo(AllInfo);
}

MainWindow::~MainWindow()
{
    //
}

void MainWindow::reloadInfo(InfoGroup gr)
{
    if (gr & GeneralInfo)
    {
        inst->timer.setInterval(Global::timerInterval());
    }
    if (gr | ManorInfo)
    {
        inst->manorTimer.setInterval(inst->manorEtimer.isValid() ? Global::olympiadCheckInterval() :
                                                                   Global::manorTimerInterval());
        inst->chatRowCount = Global::chatRowCount();
    }
    if (gr | OlympiadMessageInfo)
    {
        QString fn = BDirTools::findResource("olympiad_message.png", BDirTools::UserOnly);
        inst->olympiadMessageMask = QImage(fn).createAlphaMask();
    }
    if (gr | FishingInfo)
    {
        inst->fishingKeys = Global::fishingKeyList();
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    mustExit = true;
    if (loop)
        loop->exit();
    bSettings->setValue("GUI/main_window_geometry", saveGeometry());
    bSettings->setValue("GUI/main_window_state", saveState());
    bSettings->setValue("GUI/splitter_state", spltr->saveState());
    bSettings->setValue("GUI/stay_on_top", cboxOnTop->isChecked());
    return QMainWindow::closeEvent(e);
}

bool MainWindow::testPecked(bool anyHp)
{
    QImage s = Global::grabDesktop(fishHpPos, 1, 11);
    return s == fishHp || (anyHp && s == fishHpBackground);
}

bool MainWindow::testTarget()
{
    QImage s = Global::grabDesktop(targetClosePos, 14, 14);
    return s == targetClose;
}

int MainWindow::getFishHp()
{
    QImage s = Global::grabDesktop(fishHpPos, 230, 11);
    for (int i = 0; i < 230; ++i)
        if (s.copy(i, 0, 1, 11) == fishHpBackground)
            return i;
    return 230;
}

void MainWindow::useReeling()
{
    Global::emulateKeyPress(fishingKey(UseFishingShot));
    waitMsecs(200, loop);
    if (mustExit)
        return;
    else
        loop = 0;
    Global::emulateKeyPress(fishingKey(UseReeling));
}

void MainWindow::usePumping()
{
    Global::emulateKeyPress(fishingKey(UseFishingShot));
    waitMsecs(200, loop);
    if (mustExit)
        return;
    else
        loop = 0;
    Global::emulateKeyPress(fishingKey(UsePumping));
}

void MainWindow::resetInterface(bool enabled)
{
    wgtAutoManor->setEnabled(enabled);
    wgtFishing->setEnabled(enabled);
    QString s = tr("Game area position:", "lbl text") + " ";
    if (windowPos.x() > 0 && windowPos.y() > 0)
        s += "<b>(" + QString::number(windowPos.x()) + "; " + QString::number(windowPos.y()) + ")</b>";
    else
        s += "<font color=red>" + tr("Not detected", "lbl text") + "</font>";
    /*s += "<br>" + tr("Fish HP position:", "lbl text") + " ";
    if (fishHpPos.x() > 0 && fishHpPos.y() > 0)
        s += "<b>(" + QString::number(fishHpPos.x()) + "; " + QString::number(fishHpPos.y()) + ")</b>";
    else
        s += "<font color=red>" + tr("Not detected", "lbl text") + "</font>";
    s += "<br>" + tr("Target close button position:", "lbl text") + " ";
    if (targetClosePos.x() > 0 && targetClosePos.y() > 0)
        s += "<b>(" + QString::number(targetClosePos.x()) + "; " + QString::number(targetClosePos.y()) + ")</b>";
    else
        s += "<font color=red>" + tr("Not detected", "lbl text") + "</font>";
    s += "<br>" + tr("Chat bottom left edge position:", "lbl text") + " ";
    if (chatBottomPos.x() > 0 && chatBottomPos.y() > 0)
        s += "<b>(" + QString::number(chatBottomPos.x()) + "; " + QString::number(chatBottomPos.y()) + ")</b>";
    else
        s += "<font color=red>" + tr("Not detected", "lbl text") + "</font>";*/
    lblInfo->setText(s);
}

void MainWindow::logFishing(const QString &text)
{
    if (Qt::mightBeRichText(text))
        ptedtFishing->appendHtml(text);
    else
        ptedtFishing->appendPlainText(text);
}

bool MainWindow::wait(int msecs)
{
    waitMsecs(msecs, loop);
    if (mustExit)
        fishing = false;
    else
        loop = 0;
    return !mustExit;
}

bool MainWindow::isInterfaceDetected()
{
    return windowPos.x() > 0 && windowPos.y() > 0 && fishHpPos.x() > 0 && fishHpPos.y() > 0
            && targetClosePos.x() > 0 && targetClosePos.y() > 0 && chatBottomPos.x() > 0 && chatBottomPos.y() > 0;
}

QString MainWindow::fishingKey(FishingAction a)
{
    if (a >= fishingKeys.size())
        return "";
    int k = fishingKeys.at(a);
    if (!k)
        return "";
    return "F" + QString::number(k);
}

void MainWindow::retranslateUi()
{
    setWindowTitle("Cute LA2");
    //menus
    mmnuFile->setTitle(tr("File", "mnu title"));
    mactQuit->setText(tr("Quit", "act text"));
    mmnuEdit->setTitle(tr("Edit", "mnu title"));
    mmnuHelp->setTitle(tr("Help", "mnuTitle"));
    //
    btnManor->setText(manorTimer.isActive() ? trManorTurnOff : trManorTurnOn);
    btnTimerStart->setText(tr("Start", "btn text"));
    btnTimerPause->setText(timer.isActive() ? trTimerPause : trTimerUnpause);
    //
    resetInterface(false);
    cboxOnTop->setText(tr("Stay on top", "cbox text"));
    btnDetect->setText(tr("Detect game interface", "btn text"));
    //
    twgtManor->setTabText(0, tr("Manor", "twgt tab text"));
    twgtManor->setTabText(1, tr("Timer", "twgt tab text"));
    twgtBots->setTabText(0, tr("Fishing", "twgt tab text"));
    //
    btnFishing->setText(fishingActive ? trFishingStop : trFishingStart);
}

void MainWindow::timerTimeout()
{
    qint64 r = timerMsecs + etimer.elapsed();
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
    lblTimer->setText(h + ":" + m + ":" + s + ":" + ms);
}

void MainWindow::manorTimerTimeout()
{
    if (!manorEtimer.isValid())
    {
        for (int i = 0; i < chatRowCount; ++i)
        {
            QPoint p = chatBottomPos + QPoint(0, -12) + QPoint(0, -15 * i);
            int w = olympiadMessageMask.width();
            QImage s = Global::grabDesktop(p, w, 11);
            QRgb rr = Global::getMainColor(s);
            s = Global::cutExtraSpace(Global::removeNoise(s, &rr), &rr);
            if (s.createAlphaMask() == olympiadMessageMask)
            {
                btnManor->setEnabled(false);
                manorTimerMsecs = 6 * BeQt::Minute + 100;
                manorEtimer.start();
                manorTimer.setInterval(Global::manorTimerInterval());
                manorTimerTimeout();
                break;
            }
        }
    }
    else
    {
        qint64 r = manorTimerMsecs - manorEtimer.elapsed();
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
            lblAutoManor->setText(m + ":" + s + ":" + ms);
        }
        else
        {
            manorTimer.stop();
            manorEtimer.invalidate();
            lblAutoManor->setText("The time has come!");
            Global::emulateKeyPress("Ctrl+Alt+C");
            btnManor->setEnabled(true);
        }
    }
}

void MainWindow::btnManorClicked()
{
    if (manorTimer.isActive())
        manorTimer.stop();
    else
        manorTimer.start();
    btnManor->setText(manorTimer.isActive() ? trManorTurnOff : trManorTurnOn);
}

void MainWindow::btnTimerStartClicked()
{
    etimer.invalidate();
    etimer.start();
    timer.stop();
    btnTimerPause->setEnabled(true);
    btnTimerPause->setText(trTimerPause);
    timerMsecs = 0;
    timer.start();
}

void MainWindow::btnTimerPauseClicked()
{
    if (timer.isActive())
    {
        timer.stop();
        timerMsecs += etimer.elapsed();
        etimer.invalidate();
        btnTimerPause->setText(trTimerUnpause);
    }
    else
    {
        timer.start();
        etimer.invalidate();
        etimer.start();
        btnTimerPause->setText(trTimerPause);
    }
}

void MainWindow::cboxToggled(bool b)
{
    if (b)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else
        setWindowFlags(windowFlags() & ~Qt::WindowStaysOnTopHint);
    show();
}

void MainWindow::btnDetectClicked()
{
    if (isInterfaceDetected())
    {
        QMessageBox msg;
        msg.setWindowTitle(tr("Confirmation", "msgbox windowTitle"));
        msg.setIcon(QMessageBox::Question);
        msg.setText(tr("The game interface is already detected. Do you want to detect it again?", "msgbox text"));
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msg.setDefaultButton(QMessageBox::Yes);
        if (msg.exec() != QMessageBox::Yes)
            return;
    }
    btnDetect->setEnabled(false);
    lblInfo->setText(tr("Detecting game interface...", "lbl text"));
    const QPoint FishHPOffset = QPoint(13, 236);
    const QPoint TargetCloseOffset = QPoint(-25, 6);
    const QPoint ChatOffset = QPoint(24, -55);
    const QSettings info(Global::gameDir() + "/system/WindowsInfo.ini", QSettings::IniFormat);
    int fishX = info.value("FishViewportWnd/posX").toInt();
    int fishY = info.value("FishViewportWnd/posY").toInt();
    int targetX = info.value("TargetStatusWnd/posX").toInt();
    int targetY = info.value("TargetStatusWnd/posY").toInt();
    int targetW = info.value("TargetStatusWnd/width").toInt();
    int targetH = info.value("TargetStatusWnd/height").toInt();
    const QSettings option(Global::gameDir() + "/system/Option.ini", QSettings::IniFormat);
    int x = option.value("Video/GamePlayViewportX").toInt();
    int y = option.value("Video/GamePlayViewportY").toInt();
    windowPos = Global::detectWindowPosition(Global::detectionDelay());
    fishHpPos = windowPos;
    targetClosePos = windowPos;
    chatBottomPos = windowPos;
    if (windowPos.x() >= 0 && windowPos.y() >= 0 && fishX > 0 && fishY > 0)
        fishHpPos += FishHPOffset + QPoint(fishX, fishY);
    else
        fishHpPos = QPoint(-1, -1);
    if (windowPos.x() > 0 && windowPos.y() > 0 && targetX >= 0 && targetY >= 0 && targetW >= 0 && targetH >= 0)
        targetClosePos += TargetCloseOffset + QPoint(targetX, targetY) + QPoint(targetW, 0);
    else
        targetClosePos = QPoint(-1, -1);
    if (windowPos.x() >= 0 && windowPos.y() >= 0 && x > 0 && y > 0)
        chatBottomPos += QPoint(0, y) + ChatOffset;
    else
        chatBottomPos = QPoint(-1, -1);
    resetInterface(isInterfaceDetected());
    btnDetect->setEnabled(true);
}

void MainWindow::btnFishingClicked()
{
    fishingActive = !fishingActive;
    btnFishing->setText(fishingActive ? trFishingStop : trFishingStart);
    if (!fishingActive || fishing)
        return;
    fishing = true;
    int delay = Global::fishingStartDelay();
    logFishing("Preparing to fish. Waiting for " + QString::number(delay) + " seconds...");
    waitMsecs(delay * BeQt::Second, loop);
    if (mustExit)
    {
        fishing = false;
        return;
    }
    else
        loop = 0;
    if (!fishingActive)
    {
        fishing = false;
        return logFishing("Fishing cancelled.");
    }
    logFishing("Switching to fishing panel...");
    Global::emulateKeyPress("Alt+F" + QString::number(Global::fishingPanelNumber()));
    if (!wait(1000))
        return;
    if (Global::fishingEquipBeforeStart())
    {
        logFishing("Equipping fishing gear...");
        Global::emulateKeyPress(fishingKey(EquipRod));
        if (!wait(1000))
            return;
        Global::emulateKeyPress(fishingKey(EquipBait));
        if (!wait(1000))
            return;
    }
    while (fishingActive)
    {
        logFishing("<font color=blue>Starting fishing...</font>");
        Global::emulateKeyPress(fishingKey(UseFishing));
        volatile bool pecked = false;
        logFishing("Waiting for a fish to peck...");
        for (int i = 0; i < 200; ++i)
        {
            if (!wait(100))
                return;
            if (testPecked())
            {
                logFishing("<font color=blue>A fish pecked!</font>");
                pecked = true;
                break;
            }
        }
        if (!pecked)
        {
            logFishing("<font color=red>No fish pecked.</font> Waiting a bit...");
            if (!wait(2000))
                return;
            logFishing("<font color=green>Cycle finished.</font>");
            continue;
        }
        if (!wait(1000))
            return;
        while (testPecked())
        {
            logFishing("Deciding which skill to use...");
            int prevHP = getFishHp();
            if (!wait(500))
                return;
            if (!testPecked())
            {
                logFishing("No need to use skills.");
                break;
            }
            if (getFishHp() > prevHP)
            {
                logFishing("<font color=magenta>Using Reeling...</font>");
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
                    logFishing("No need to use skills.");
                    break;
                }
                if (getFishHp() > prevHP)
                {
                    logFishing("<font color=magenta>Using Reeling...</font>");
                    useReeling();
                    if (!wait(1500))
                        return;
                }
                else
                {
                    if (testPecked(false))
                    {
                        logFishing("<font color=orange>Using Pumping...</font>");
                        usePumping();
                    }
                    else
                    {
                        logFishing("No need to use skills.");
                    }
                    if (!wait(1000))
                        return;
                }
            }
        }
        logFishing("<font color=green>Fishing finished!</font> Waiting for a possible attack...");
        if (!wait(2000))
            return;
        if (testTarget())
        {
            logFishing("<font color=blue>A monster attacks!</font> Equipping arms and counterattacking...");
            Global::emulateKeyPress(fishingKey(EquipWeapon) + "," + fishingKey(Attack));
            while (testTarget())
                if (!wait(100))
                    return;
            logFishing("<font color=blue>The monster is killed!</font> Equipping fishing gear...");
            //TODO: While collecting dropped items, a character may turn away from the water.
            /*ptedt->appendPlainText("Collecting dropped items...");
            for (int i = 0; i < 3; ++i)
            {
#if defined(Q_OS_WIN)
                keybd_event(Qt::Key_5, 0, 0, 0);
                keybd_event(Qt::Key_5, 0, KEYEVENTF_KEYUP, 0);
#endif
                waitMsecs(500);
            }*/
            Global::emulateKeyPress(fishingKey(EquipRod));
            if (!wait(1000))
                return;
            Global::emulateKeyPress(fishingKey(EquipBait));
            if (!wait(1000))
                return;
            //The following is needed to make fishing possible again (LA2 bug?)
            Global::emulateKeyPress(fishingKey(SitStand));
            if (!wait(4000))
                return;
            Global::emulateKeyPress(fishingKey(SitStand));
            if (!wait(4000))
                return;
        }
        else
        {
            logFishing("No monster. Waiting a bit...");
        }
        if (!wait(2000))
            return;
        logFishing("<font color=green>Cycle finished.</font>");
    }
    logFishing("Stopped fishing.");
    fishing = false;
}

MainWindow *MainWindow::inst = 0;
