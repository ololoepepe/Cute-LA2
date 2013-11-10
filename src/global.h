#ifndef GLOBAL_H
#define GLOBAL_H

#include <BeQt>

#include <QImage>
#include <QList>

class QPoint;
class QString;
class QKeySequence;

namespace Global
{

typedef QList<int> FishingKeyList;

enum SearchDirection
{
    LeftToRightTopToBottom,
    RightToLeftTopToBottom,
    LeftToRightBottomToTop,
    RightToLeftBottomToTop,
    TopToBottomLeftToRight,
    BottomToTopLeftToRight,
    TopToBottomRightToLeft,
    BottomToTopRightToLeft/*,
    UpperLeftToBottomRightTopToBottom,
    UpperLeftToBottomRightBottomToTop,
    UpperRightToBottomLeftTopToBottom,
    UpperRightToBottomLeftBottomToTop,
    BottomLeftToUpperRightTopToBottom,
    BottomLeftToUpperRightBottomToTop,
    BottomRightToUpperLeftTopToBottom,
    BottomRightToUpperLeftBottomToTop*/
};

QImage grabOlympiadMessage();
QPoint selectManorButtonPos(const QPoint &previousPos = QPoint(-1, -1));
void emulateKeyPress(const QKeySequence &key);
void emulateKeyPress(const QString &key);
void emulateMouseClick(Qt::MouseButton button, int x = -1, int y = -1);
void emulateMouseClick(Qt::MouseButton button, const QPoint &pos);
QImage grabDesktop(int x = 0, int y = 0, int w = -1, int h = -1);
QImage grabDesktop(const QPoint &pos, int w = -1, int h = -1);
QRgb getMainColor(const QImage &img);
QImage removeNoise(const QImage &src, QRgb *mainColor = 0);
QImage cutExtraSpace(const QImage &img, QRgb *mainColor);
QPoint positionIn(const QImage &search, const QImage &templ, SearchDirection direction = LeftToRightTopToBottom);
//
void setGameDir(const QString &dir);
void setTimerInterval(int msecs);
void setDetectionDelay(int seconds);
void setDetectWindowID(bool b);
QString gameDir();
int timerInterval();
int detectionDelay();
bool detectWindowID();
//
void setOlympiadCheckInterval(int msecs);
void setManorTimerInterval(int msecs);
void setChatRowCount(int n);
void setManorButtonPos(const QPoint &pos);
bool setOlympiadMessageTemplate(const QImage &src);
int olympiadCheckInterval();
int manorTimerInterval();
int chatRowCount();
QPoint manorButtonPos();
QImage olympiadMessageMask();
//
void setFishingEquipBeforeStart(bool b);
void setFishingStartDelay(int seconds);
void setFishingPanelNumber(int n);
void setMainPanelNumber(int n);
void setFishingKeyList(const FishingKeyList &list);
bool fishingEquipBeforeStart();
int fishingStartDelay();
int fishingPanelNumber();
int mainPanelNumber();
FishingKeyList fishingKeyList();
//
QPoint detectWindowPosition(int delayMsecs = 5 * BeQt::Second);

}

#endif // GLOBAL_H
