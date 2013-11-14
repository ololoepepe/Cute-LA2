#include "mainwindow.h"
#include "global.h"
#include "timerwidget.h"
#include "manorwidget.h"
#include "fishingwidget.h"
#include "craftwidget.h"

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
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrentMap>
#include <QTime>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    spltr = new QSplitter(Qt::Vertical);
    spltr->setObjectName("splitter");
      QWidget *wgt = new QWidget;
        QVBoxLayout *vlt = new QVBoxLayout(wgt);
          twgtManor = new QTabWidget;
            twgtManor->setTabPosition(QTabWidget::West);
            wgtManor = new ManorWidget;
              wgtManor->setEnabled(false);
            twgtManor->addTab(wgtManor, "");
            twgtManor->addTab(new TimerWidget, "");
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
        wgtFishing = new FishingWidget;
          wgtFishing->setEnabled(false);
        twgtBots->addTab(wgtFishing, "");
        wgtCraft = new CraftWidget;
          wgtCraft->setEnabled(false);
        twgtBots->addTab(wgtCraft, "");
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
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    FishingWidget::quitLoop();
    bSettings->setValue("GUI/main_window_geometry", saveGeometry());
    bSettings->setValue("GUI/main_window_state", saveState());
    bSettings->setValue("GUI/splitter_state", spltr->saveState());
    bSettings->setValue("GUI/stay_on_top", cboxOnTop->isChecked());
    return QMainWindow::closeEvent(e);
}

void MainWindow::resetInterface(bool enabled)
{
    twgtBots->widget(0)->setEnabled(enabled);
    wgtManor->setEnabled(enabled);
    wgtFishing->setEnabled(enabled);
    wgtCraft->setEnabled(enabled);
    QString s = tr("Game area position:", "lbl text") + " ";
    QPoint pos = Global::windowPos();
    if (Global::isGameInterfaceDetected())
        s += "<b>(" + QString::number(pos.x()) + "; " + QString::number(pos.y()) + ")</b>";
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

void MainWindow::retranslateUi()
{
    setWindowTitle("Cute LA2");
    //menus
    mmnuFile->setTitle(tr("File", "mnu title"));
    mactQuit->setText(tr("Quit", "act text"));
    mmnuEdit->setTitle(tr("Edit", "mnu title"));
    mmnuHelp->setTitle(tr("Help", "mnuTitle"));
    //
    resetInterface(false);
    cboxOnTop->setText(tr("Stay on top", "cbox text"));
    btnDetect->setText(tr("Detect game interface", "btn text"));
    //
    twgtManor->setTabText(0, tr("Manor", "twgt tab text"));
    twgtManor->setTabText(1, tr("Timer", "twgt tab text"));
    twgtBots->setTabText(0, tr("Fishing", "twgt tab text"));
    twgtBots->setTabText(1, tr("Craft", "twgt tab text"));
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
    if (Global::isGameInterfaceDetected())
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
    bool b = Global::detectGameInterface();
    resetInterface(b);
    btnDetect->setEnabled(true);
}
