#ifndef GLOBAL_H
#define GLOBAL_H

#include <BeQt>

#include <QImage>
#include <QList>

class QPoint;
class QString;
class QKeySequence;
class QTime;
class QWidget;

namespace Global
{

typedef QList<int> FishingKeyList;

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

bool grabOlympiadMessage();
bool selectManorButtonPos();
bool selectCraftButtonPos();
void emulateKeyPress(const QKeySequence &key);
void emulateKeyPress(const QString &key);
void emulateMouseClick(Qt::MouseButton button, int x = -1, int y = -1);
void emulateMouseClick(Qt::MouseButton button, const QPoint &pos);
QImage grabDesktop(int x = 0, int y = 0, int w = -1, int h = -1);
QImage grabDesktop(const QPoint &pos, int w = -1, int h = -1);
QRgb getMainColor(const QImage &img);
QImage removeNoise(const QImage &src, QRgb *mainColor = 0);
QImage cutExtraSpace(const QImage &img, QRgb *mainColor);
QPoint positionIn(const QImage &search, const QImage &templ);
bool detectGameInterface();
bool isGameInterfaceDetected();
QPoint windowPos();
QPoint fishHpPos();
QPoint targetClosePos();
QPoint chatBottomPos();
void switchToWindow();
//
void loadSettings();
void saveSettings();
//
void setGameDir(const QString &dir);
void setTimerInterval(int msecs);
void setDetectionDelay(int seconds);
void setDetectWindowID(bool b);
void setOlympiadCheckInterval(int msecs);
void setManorTimerInterval(int msecs);
void setChatRowCount(int n);
void setManorAutoStartEnabled(bool enabled);
void setManorAutoStartTime(const QTime &t);
void setManorTimeCorrection(int msecs);
void setFishingEquipBeforeStart(bool b);
void setFishingStartDelay(int seconds);
void setFishingPanelNumber(int n);
void setMainPanelNumber(int n);
void setFishingKeyList(const FishingKeyList &list);
void setMpRegen(int x);
void setMpConsumption(int x);
void setCraftStartDelay(int seconds);
QString gameDir();
int timerInterval();
int detectionDelay();
bool detectWindowID();
int olympiadCheckInterval();
int manorTimerInterval();
int chatRowCount();
bool manorAutoStartEnabled();
QTime manorAutoStartTime();
int manorTimeCorrection();
QPoint manorButtonPos();
bool fishingEquipBeforeStart();
int fishingStartDelay();
int fishingPanelNumber();
int mainPanelNumber();
FishingKeyList fishingKeyList();
QString fishingKey(FishingAction a);
QPoint craftButtonPos();
int mpRegen();
int mpConsumption();
int craftStartDelay();
//
const QImage *olympiadMessageMask();

}

#endif // GLOBAL_H
