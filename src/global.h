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
void emulateKeyPress(const QKeySequence &key);
void emulateKeyPress(const QString &key);
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
QString gameDir();
int timerInterval();
int detectionDelay();
//
void setOlympiadCheckInterval(int msecs);
void setManorTimerInterval(int msecs);
void setChatRowCount(int n);
int olympiadCheckInterval();
int manorTimerInterval();
int chatRowCount();
//
void setFishingEquipBeforeStart(bool b);
void setFishingStartDelay(int seconds);
void setFishingPanelNumber(int n);
void setFishingKeyList(const FishingKeyList &list);
bool fishingEquipBeforeStart();
int fishingStartDelay();
int fishingPanelNumber();
FishingKeyList fishingKeyList();
//
QPoint detectWindowPosition(int delayMsecs = 5 * BeQt::Second);

}

#endif // GLOBAL_H
