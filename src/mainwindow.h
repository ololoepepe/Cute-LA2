#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QLabel;
class QPushButton;
class QPlainTextEdit;
class QAction;
class QMenu;
class QTabWidget;
class QCheckBox;
class QCloseEvent;
class QString;
class QEventLoop;
class QWidget;
class QSplitter;

#include "global.h"

#include <BTranslation>

#include <QMainWindow>
#include <QImage>
#include <QTimer>
#include <QElapsedTimer>
#include <QPoint>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public:
    enum InfoGroup
    {
        GeneralInfo = 1,
        ManorInfo = 2,
        OlympiadMessageInfo = 4,
        ManorButtonInfo = 8,
        FullManorInfo = ManorInfo | OlympiadMessageInfo | ManorButtonInfo,
        FishingInfo = 16,
        AllInfo = GeneralInfo | FullManorInfo | FishingInfo
    };
public:
    static void reloadInfo(InfoGroup gr);
protected:
    void closeEvent(QCloseEvent *e);
private:
    enum FishingAction
    {
        EquipRod = 0,
        EquipBait,
        UseFishing,
        UseReeling,
        UsePumping,
        UseFishingShot,
        Attack,
        SitStand,
        EquipWeapon
    };
private:
    bool testPecked(bool anyHp = true);
    bool testTarget();
    int getFishHp();
    void useReeling();
    void usePumping();
    void resetInterface(bool enabled);
    void logFishing(const QString &text);
    bool wait(int msecs);
    bool isInterfaceDetected();
    QString fishingKey(FishingAction a);
private slots:
    void retranslateUi();
    void timerTimeout();
    void manorTimerTimeout();
    void btnManorClicked();
    void btnTimerStartClicked();
    void btnTimerPauseClicked();
    void cboxToggled(bool b);
    void btnDetectClicked();
    void btnFishingClicked();
private:
    static MainWindow *inst;
private:
    QSplitter *spltr;
      //vlt
        QTabWidget *twgtManor;
          QWidget *wgtAutoManor;
            QLabel *lblAutoManor;
            //hlt
              QPushButton *btnManor;
          //vlt
            QLabel *lblTimer;
            //hlt
              QPushButton *btnTimerStart;
              QPushButton *btnTimerPause;
      //vlt
        QLabel *lblInfo;
        //hlt
          QCheckBox *cboxOnTop;
          QPushButton *btnSettings;
          QPushButton *btnDetect;
      QTabWidget *twgtBots;
        QWidget *wgtFishing;
          //hlt
            QPushButton *btnFishing;
          QPlainTextEdit *ptedtFishing;
        QWidget *wgtLeveling;
          //hlt
            QPushButton *btnLeveling;
          QPlainTextEdit *ptedtLeveling;
    //
    QMenu *mmnuFile;
      QAction *mactQuit;
    QMenu *mmnuEdit;
    QMenu *mmnuHelp;
    //
    BTranslation trManorTurnOn;
    BTranslation trManorTurnOff;
    BTranslation trTimerPause;
    BTranslation trTimerUnpause;
    BTranslation trFishingStart;
    BTranslation trFishingStop;
    //
    WId windowID;
    Global::FishingKeyList fishingKeys;
    QImage targetClose;
    QImage olympiadMessageMask;
    bool fishingActive;
    bool fishing;
    bool mustExit;
    int chatRowCount;
    QTimer timer;
    QTimer manorTimer;
    QElapsedTimer etimer;
    QElapsedTimer manorEtimer;
    qint64 timerMsecs;
    qint64 manorTimerMsecs;
    QEventLoop *loop;
    QPoint windowPos;
    QPoint fishHpPos;
    QPoint targetClosePos;
    QPoint chatBottomPos;
    QPoint manorButtonPos;
};

#endif // MAINWINDOW_H
