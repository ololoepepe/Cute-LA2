#include "global.h"
#include "mainwindow.h"
#include "timerwidget.h"
#include "manorwidget.h"

#include <BeQtGlobal>
#include <BeQt>
#include <BApplication>
#include <BDirTools>

#include <QImage>
#include <QPoint>
#include <QMap>
#include <QApplication>
#include <QDesktopWidget>
#include <QPixmap>
#include <QSettings>
#include <QVariant>
#include <QString>
#include <QKeySequence>
#include <QFrame>
#include <QDialog>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QToolButton>
#include <QHBoxLayout>
#include <QColor>
#include <QBitmap>
#include <QIcon>
#include <QPalette>
#include <QScreen>
#include <QCursor>
#include <QTime>

#include <QDebug>

#if defined(Q_OS_WIN)
#include "windows.h"
#endif

namespace Global
{

class Frame : public QDialog
{
    Q_DECLARE_TR_FUNCTIONS(Frame)
public:
    enum Shape
    {
        FrameShape,
        SightShape
    };
public:
    explicit Frame(Shape s);
protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
private:
    bool pressed;
    QPoint dpos;
};

Frame::Frame(Shape s)
{
    pressed = false;
    switch (s)
    {
    case FrameShape:
    {
        QHBoxLayout *hlt = new QHBoxLayout(this);
          hlt->setContentsMargins(0, 0, 0, 0);
          hlt->setSpacing(0);
          QFrame *fr = new QFrame;
            fr->setFrameShape(QFrame::Box);
            fr->setLineWidth(4);
            fr->setStyleSheet("background:transparent;");
            fr->setAttribute(Qt::WA_TranslucentBackground);
            QPalette p = fr->palette();
            p.setColor(QPalette::Foreground, QColor("red"));
            fr->setPalette(p);
          hlt->addWidget(fr);
          fr = new QFrame;
            fr->setFrameShape(QFrame::VLine);
            fr->setFixedWidth(12);
            fr->setLineWidth(12);
            fr->setPalette(p);
          hlt->addWidget(fr);
          QToolButton *tbtn = new QToolButton;
            tbtn->setIconSize(QSize(12, 12));
            tbtn->setToolTip(tr("Grab", "tbtn toolTip"));
            tbtn->setIcon(BApplication::icon("ok"));
            connect(tbtn, SIGNAL(clicked()), this, SLOT(accept()));
          hlt->addWidget(tbtn);
          tbtn = new QToolButton;
            tbtn->setIconSize(QSize(12, 12));
            tbtn->setToolTip(tr("Cancel", "tbtn toolTip"));
            tbtn->setIcon(BApplication::icon("editdelete"));
            connect(tbtn, SIGNAL(clicked()), this, SLOT(reject()));
          hlt->addWidget(tbtn);
        //setStyleSheet("background:transparent;");
        setAttribute(Qt::WA_TranslucentBackground);
        setWindowFlags(Qt::FramelessWindowHint);
        //
        setFixedSize(250, 23);
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        break;
    }
    case SightShape:
    {
        QVBoxLayout *vlt = new QVBoxLayout(this);
          vlt->setContentsMargins(0, 0, 0, 0);
          QHBoxLayout *hlt = new QHBoxLayout;
            hlt->addStretch();
            QToolButton *tbtn = new QToolButton;
              tbtn->setIconSize(QSize(12, 12));
              tbtn->setToolTip(tr("Select", "tbtn toolTip"));
              tbtn->setIcon(BApplication::icon("ok"));
              connect(tbtn, SIGNAL(clicked()), this, SLOT(accept()));
            hlt->addWidget(tbtn);
            tbtn = new QToolButton;
              tbtn->setIconSize(QSize(12, 12));
              tbtn->setToolTip(tr("Cancel", "tbtn toolTip"));
              tbtn->setIcon(BApplication::icon("editdelete"));
              connect(tbtn, SIGNAL(clicked()), this, SLOT(reject()));
            hlt->addWidget(tbtn);
          vlt->addLayout(hlt);
          vlt->addStretch();
          hlt = new QHBoxLayout;
            hlt->addStretch();
            QFrame *fr = new QFrame;
              fr->setFrameShape(QFrame::Box);
              fr->setLineWidth(20);
              QPalette p = fr->palette();
              p.setColor(QPalette::Foreground, QColor("red"));
              fr->setPalette(p);
              fr->setFixedSize(16, 16);
            hlt->addWidget(fr);
          vlt->addLayout(hlt);
        fr = new QFrame(this);
          fr->setFrameShape(QFrame::HLine);
          fr->setLineWidth(4);
          fr->setPalette(p);
          fr->setFixedSize(100, 4);
          fr->move(0, 50);
        fr = new QFrame(this);
          fr->setFrameShape(QFrame::VLine);
          fr->setFixedWidth(12);
          fr->setLineWidth(12);
          fr->setPalette(p);
          fr->setFixedSize(4, 100);
          fr->move(50, 0);
        //setStyleSheet("background:transparent;");
        setAttribute(Qt::WA_TranslucentBackground);
        setWindowFlags(Qt::FramelessWindowHint);
        setFixedSize(100, 100);
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
        break;
    }
    default:
        break;
    }
}

void Frame::mousePressEvent(QMouseEvent *e)
{
    pressed = true;
    dpos = e->globalPos() - pos();
    QDialog::mousePressEvent(e);
}

void Frame::mouseReleaseEvent(QMouseEvent *e)
{
    pressed = false;
    QDialog::mouseReleaseEvent(e);
}

void Frame::mouseMoveEvent(QMouseEvent *e)
{
    if (!pressed)
        return QDialog::mouseMoveEvent(e);
    move(e->globalPos() - dpos);
    QDialog::mouseMoveEvent(e);
}

enum KeyPressMode
{
    DownOnly = 1,
    UpOnly = 2,
    DownAndUp = DownOnly | UpOnly
};

QString vGameDir = "C:/Program files (x86)/NCSoft";
int vTimerInterval = 100;
int vDetectionDelay = 5;
bool vDetectWindowID = false;
int vOlympiadCheckInterval = 10;
int vManorTimerInterval = 50;
int vChatRowCount = 4;
bool vManorAutoStartEnabled = false;
QTime vManorAutoStartTime;
int vManorTimeCorrection = 0;
QPoint vManorButtonPos = QPoint(-1, -1);
bool vFishingEquipBeforeStart = true;
int vFishingStartDelay = 5;
int vFishingPanelNumber = 1;
int vMainPanelNumber = 1;
int vRestTime = 30;
FishingKeyList vFishingKeyList = FishingKeyList() << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0 << 0;
QImage vOlympiadMessageMask;
#if defined(Q_OS_WIN)
HWND vWindowID = 0;
#endif
QPoint vWindowPos = QPoint(-1, -1);
QPoint vFishHpPos = QPoint(-1, -1);
QPoint vTargetClosePos = QPoint(-1, -1);
QPoint vChatBottomPos = QPoint(-1, -1);
QPoint vCraftButtonPos = QPoint(-1, -1);
int vMpRegen = 10;
int vMpConsumption = 10;
int vCraftStartDelay = 5;

#if defined(Q_OS_WIN)
UINT modifierToVKey(Qt::Modifier m)
{
    switch (m)
    {
    case Qt::SHIFT:
        return VK_SHIFT;
    case Qt::CTRL:
        return VK_CONTROL;
    case Qt::ALT:
        return VK_MENU;
    default:
        return 0;
    }
}

UINT keyToVKey(int key)
{
    switch (key)
    {
    case Qt::Key_Backspace:
        return VK_BACK;
    case Qt::Key_Tab:
        return VK_TAB;
    case Qt::Key_Return:
        return VK_RETURN;
    case Qt::Key_Escape:
        return VK_ESCAPE;
    case Qt::Key_Left:
        return VK_LEFT;
    case Qt::Key_Right:
        return VK_RIGHT;
    case Qt::Key_Up:
        return VK_UP;
    case Qt::Key_Down:
        return VK_DOWN;
    case Qt::Key_Insert:
        return VK_INSERT;
    case Qt::Key_Delete:
        return VK_DELETE;
    case Qt::Key_Home:
        return VK_HOME;
    case Qt::Key_End:
        return VK_END;
    case Qt::Key_PageUp:
        return VK_PRIOR;
    case Qt::Key_PageDown:
        return VK_NEXT;
    case Qt::Key_F1:
        return VK_F1;
    case Qt::Key_F2:
        return VK_F2;
    case Qt::Key_F3:
        return VK_F3;
    case Qt::Key_F4:
        return VK_F4;
    case Qt::Key_F5:
        return VK_F5;
    case Qt::Key_F6:
        return VK_F6;
    case Qt::Key_F7:
        return VK_F7;
    case Qt::Key_F8:
        return VK_F8;
    case Qt::Key_F9:
        return VK_F9;
    case Qt::Key_F10:
        return VK_F10;
    case Qt::Key_F11:
        return VK_F11;
    case Qt::Key_F12:
        return VK_F12;
    default:
        return (UINT) key;
    }
}

void emulateKeyPress(UINT key, KeyPressMode m = DownAndUp)
{
    if (m & DownOnly)
    {
        INPUT input;
        input.type = INPUT_KEYBOARD;
        input.ki.time = 0;
        input.ki.wVk = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.dwFlags = KEYEVENTF_SCANCODE;
        input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
        SendInput(1, &input, sizeof(INPUT));
    }
    Sleep(1);
    if (m & UpOnly)
    {
        INPUT input;
        input.type = INPUT_KEYBOARD;
        input.ki.time = 0;
        input.ki.wVk = 0;
        input.ki.dwExtraInfo = 0;
        input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        input.ki.wScan = MapVirtualKey(key, MAPVK_VK_TO_VSC);
        SendInput(1, &input, sizeof(INPUT));
    }
}
#endif

bool grabOlympiadMessage()
{
    Frame *f = new Frame(Frame::FrameShape);
    f->exec();
    f->deleteLater();
    if (QDialog::Accepted != f->result())
        return false;
    QImage img = grabDesktop(f->pos() + QPoint(4, 4), 186, 15);
    QRgb rr = getMainColor(img);
    img = cutExtraSpace(removeNoise(img, &rr), &rr);
    QString fn = BApplication::location(BApplication::DataPath, BApplication::UserResources) + "/olympiad_message.png";
    bool b = img.save(fn, "png");
    if (b)
        vOlympiadMessageMask = img.createAlphaMask();
    return b;
}

bool selectManorButtonPos()
{
    Frame *f = new Frame(Frame::SightShape);
    if (vManorButtonPos.x() > 0 && vManorButtonPos.y() > 0)
        f->move(vManorButtonPos + QPoint(-50, -50));
    f->exec();
    f->deleteLater();
    if (QDialog::Accepted != f->result())
        return false;
    vManorButtonPos = f->pos() + QPoint(50, 50);
    return true;
}

bool selectCraftButtonPos()
{
    Frame *f = new Frame(Frame::SightShape);
    if (vCraftButtonPos.x() > 0 && vCraftButtonPos.y() > 0)
        f->move(vCraftButtonPos + QPoint(-50, -50));
    f->exec();
    f->deleteLater();
    if (QDialog::Accepted != f->result())
        return false;
    vCraftButtonPos = f->pos() + QPoint(50, 50);
    return true;
}

void emulateKeyPress(const QKeySequence &key)
{
    typedef QList<Qt::Modifier> ModifierList;
    init_once(ModifierList, modifiers, ModifierList())
    {
        modifiers << Qt::SHIFT << Qt::META << Qt::CTRL << Qt::ALT;
    }
    if (!key.count())
        return;
#if defined(Q_OS_WIN)
    foreach (int i, bRangeD(0, key.count() - 1))
    {
        int k = key[(uint) i];
        int kk = k;
        foreach (Qt::Modifier m, modifiers)
        {
            if (m & k)
            {
                kk &= ~m;
                emulateKeyPress(modifierToVKey(m), DownOnly);
                Sleep(10);
            }
        }
        emulateKeyPress(keyToVKey(kk));
        foreach (Qt::Modifier m, modifiers)
        {
            if (m & k)
            {
                Sleep(10);
                emulateKeyPress(modifierToVKey(m), UpOnly);
            }
        }
    }
#endif
}

void emulateKeyPress(const QString &key)
{
    emulateKeyPress(QKeySequence(key));
}

void emulateMouseClick(Qt::MouseButton button, int x, int y)
{
    Q_UNUSED(button)
    QPoint pos = QCursor::pos();
    if (x >= 0)
        pos.setX(x);
    if (y >= 0)
        pos.setY(y);
    if (x >= 0 || y >= 0)
        QCursor::setPos(pos);
#if defined(Q_OS_WIN)
    Sleep(10);
    switch (button)
    {
    case Qt::LeftButton:
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        break;
    case Qt::RightButton:
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        break;
    case Qt::MiddleButton:
        mouse_event(MOUSEEVENTF_MIDDLEDOWN, 0, 0, 0, 0);
        break;
    default:
        return;
    }
    switch (button)
    {
    case Qt::LeftButton:
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
        break;
    case Qt::RightButton:
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
        break;
    case Qt::MiddleButton:
        mouse_event(MOUSEEVENTF_MIDDLEUP, 0, 0, 0, 0);
        break;
    default:
        return;
    }
#endif
}

void emulateMouseClick(Qt::MouseButton button, const QPoint &pos)
{
    emulateMouseClick(button, pos.x(), pos.y());
}

QImage grabDesktop(int x, int y, int w, int h)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    QPixmap pm = QPixmap::grabWindow(QApplication::desktop()->winId(), x, y, w, h);
#else
    QPixmap pm = QApplication::primaryScreen()->grabWindow(QApplication::desktop()->winId(), x, y, w, h);
#endif
    return pm.toImage().convertToFormat(QImage::Format_RGB32);
}

QImage grabDesktop(const QPoint &pos, int w, int h)
{
    return grabDesktop(pos.x(), pos.y(), w, h);
}

QRgb getMainColor(const QImage &img)
{
    QSet<QRgb> set;
    for (int x = 0; x < img.width(); ++x)
    {
        for (int y = 0; y < img.height(); ++y)
        {
            QRgb r = img.pixel(x, y);
            if (!set.contains(r))
                set.insert(r);
        }
    }
    QMap<QRgb, int> map;
    foreach (QRgb r, set.values())
    {
        int weight = 0;
        foreach (QRgb rr, set.values())
            weight += qAbs(qRed(r) - qRed(rr)) + qAbs(qGreen(r) - qGreen(rr)) + qAbs(qBlue(r) - qBlue(rr));
        map.insert(r, weight);
    }
    int weight = 0;
    QRgb rr = 0;
    foreach (QRgb r, map.keys())
    {
        if (map.value(r) > weight)
        {
            weight = map.value(r);
            rr = r;
        }
    }
    return rr;
}

QImage removeNoise(const QImage &src, QRgb *mainColor)
{
    QRgb rr = mainColor ? *mainColor : getMainColor(src);
    QImage img = src.convertToFormat(QImage::Format_ARGB32);
    for (int x = 0; x < img.width(); ++x)
    {
        for (int y = 0; y < img.height(); ++y)
        {
            if (img.pixel(x, y) != rr)
                img.setPixel(x, y, qRgba(0, 0, 0, 0));
        }
    }
    return img;
}

QImage cutExtraSpace(const QImage &img, QRgb *mainColor)
{
    QRgb rr = mainColor ? *mainColor : getMainColor(img);
    int oddLeft = 0;
    int oddRight = 0;
    int oddTop = 0;
    int oddBottom = 0;
    for (int x = 0; x < img.width(); ++x)
    {
        bool ok = true;
        for (int y = 0; y < img.height(); ++y)
        {
            if (img.pixel(x, y) == rr)
            {
                ok = false;
                break;
            }
        }
        if (!ok)
        {
            oddLeft = x;
            break;
        }
    }
    for (int x = img.width() - 1; x >= 0; --x)
    {
        bool ok = true;
        for (int y = 0; y < img.height(); ++y)
        {
            if (img.pixel(x, y) == rr)
            {
                ok = false;
                break;
            }
        }
        if (!ok)
        {
            oddRight = img.width() - x - 1;
            break;
        }
    }
    for (int y = 0; y < img.height(); ++y)
    {
        bool ok = true;
        for (int x = 0; x < img.width(); ++x)
        {
            if (img.pixel(x, y) == rr)
            {
                ok = false;
                break;
            }
        }
        if (!ok)
        {
            oddTop = y;
            break;
        }
    }
    for (int y = img.height() - 1; y >= 0; --y)
    {
        bool ok = true;
        for (int x = 0; x < img.width(); ++x)
        {
            if (img.pixel(x, y) == rr)
            {
                ok = false;
                break;
            }
        }
        if (!ok)
        {
            oddBottom = img.height() - y - 1;
            break;
        }
    }
    return img.copy(oddLeft, oddTop, img.width() - oddLeft - oddRight, img.height() - oddTop - oddBottom);
}

QPoint positionIn(const QImage &search, const QImage &templ)
{
    if (search.size() == templ.size())
        return (search == templ) ? QPoint(0, 0) : QPoint(-1, -1);
    if (search.height() < templ.height() || search.width() < templ.width())
        return QPoint(-1, -1);
    for (int x = 0; x <= search.width() - templ.width(); ++x)
    {
        for (int y = search.height() - templ.height(); y >= 0; --y)
        {
            if (search.copy(x, y, templ.width(), templ.height()) == templ)
                return QPoint(x, y);
        }
    }
    return QPoint(-1, -1);
}

bool detectGameInterface()
{
    init_once(QImage, chatSettingsIcon, QImage())
    {
        QString fn = BDirTools::findResource("pixmaps/chat_settings.png", BDirTools::GlobalOnly);
        chatSettingsIcon = QImage(fn).convertToFormat(QImage::Format_RGB32);
    }
    static const QPoint FishHPOffset = QPoint(13, 236);
    static const QPoint TargetCloseOffset = QPoint(-25, 6);
    static const QPoint ChatOffset = QPoint(24, -55);
    if (chatSettingsIcon.isNull())
    {
        vWindowPos = QPoint(-1, -1);
        return false;
    }
    if (vGameDir.isEmpty())
    {
        vWindowPos = QPoint(-1, -1);
        return false;
    }
    const QSettings option(vGameDir + "/system/Option.ini", QSettings::IniFormat);
    int x = option.value("Video/GamePlayViewportX").toInt();
    int y = option.value("Video/GamePlayViewportY").toInt();
    const QSettings info(vGameDir + "/system/WindowsInfo.ini", QSettings::IniFormat);
    int fishX = info.value("FishViewportWnd/posX").toInt();
    int fishY = info.value("FishViewportWnd/posY").toInt();
    int targetX = info.value("TargetStatusWnd/posX").toInt();
    int targetY = info.value("TargetStatusWnd/posY").toInt();
    int targetW = info.value("TargetStatusWnd/width").toInt();
    int targetH = info.value("TargetStatusWnd/height").toInt();
    if (x < 0 || y < 0 || fishX <= 0 || fishY <= 0 || targetX < 0 || targetY < 0 || targetW <= 0 || targetH <= 0)
    {
        vWindowPos = QPoint(-1, -1);
        return false;
    }
    if (vDetectionDelay > 0)
        BeQt::waitNonBlocking(vDetectionDelay * BeQt::Second);
    QImage img = grabDesktop();
    QPoint p = positionIn(img, chatSettingsIcon);
    if (p.x() < 0 || p.y() < 0)
    {
        vWindowPos = QPoint(-1, -1);
        return false;
    }
    p.rx() -= 5;
    p.ry() += 66;
    p.ry() -= y;
    vWindowPos = p;
#if defined(Q_OS_WIN)
    vWindowID = vDetectWindowID ? GetForegroundWindow() : 0;
#endif
    vFishHpPos = vWindowPos;
    vTargetClosePos = vWindowPos;
    vChatBottomPos = vWindowPos;
    vFishHpPos += FishHPOffset + QPoint(fishX, fishY);
    vTargetClosePos += TargetCloseOffset + QPoint(targetX, targetY) + QPoint(targetW, 0);
    vChatBottomPos += QPoint(0, y) + ChatOffset;
    return true;
}

bool isGameInterfaceDetected()
{
    return vWindowPos.x() > 0 && vWindowPos.y() > 0;
}

QPoint windowPos()
{
    return isGameInterfaceDetected() ? vWindowPos : QPoint(-1, -1);
}

QPoint fishHpPos()
{
    return isGameInterfaceDetected() ? vFishHpPos : QPoint(-1, -1);
}

QPoint targetClosePos()
{
    return isGameInterfaceDetected() ? vTargetClosePos : QPoint(-1, -1);
}

QPoint chatBottomPos()
{
    return isGameInterfaceDetected() ? vChatBottomPos : QPoint(-1, -1);
}

void switchToWindow()
{
#if defined(Q_OS_WIN)
    if (vWindowID)
        SwitchToThisWindow(vWindowID, FALSE);
#endif
}

void loadSettings()
{
    vGameDir = bSettings->value("System/game_dir", "C:/Program files (x86)/NCSoft").toString();
    bool ok = false;
    int msecs = bSettings->value("System/timer_interval", 100).toInt(&ok);
    vTimerInterval = (ok && bRangeD(1, 1000).contains(msecs)) ? msecs : 100;
    ok = false;
    int seconds = bSettings->value("System/detection_delay", 5).toInt(&ok);
    vDetectionDelay = (ok && bRangeD(1, 60).contains(seconds)) ? seconds : 5;
    vDetectWindowID = bSettings->value("System/detect_window_id", false).toBool();
    ok = false;
    msecs = bSettings->value("Manor/olympiad_check_interval", 10).toInt(&ok);
    vOlympiadCheckInterval = (ok && bRangeD(1, 1000).contains(msecs)) ? msecs : 10;
    ok = false;
    msecs = bSettings->value("Manor/timer_interval", 50).toInt(&ok);
    vManorTimerInterval = (ok && bRangeD(1, 1000).contains(msecs)) ? msecs : 50;
    ok = false;
    int n = bSettings->value("Manor/chat_row_count", 4).toInt(&ok);
    vChatRowCount = (ok && bRangeD(1, 28).contains(n)) ? n : 4;
    vManorAutoStartEnabled = bSettings->value("Manor/auto_start_enabled", false).toBool();
    vManorAutoStartTime = bSettings->value("Manor/auto_start_time", QTime(20, 00)).toTime();
    ok = false;
    msecs = bSettings->value("Manor/time_correction", 0).toInt(&ok);
    vManorTimeCorrection = (ok && bRangeD(-10000, 10000).contains(msecs)) ? msecs : 0;
    vManorButtonPos = bSettings->value("Manor/button_pos", QPoint(-1, -1)).toPoint();
    vFishingEquipBeforeStart = bSettings->value("Fishing/equip_before_start", true).toBool();
    ok = false;
    seconds = bSettings->value("Fishing/start_delay", 5).toInt(&ok);
    vFishingStartDelay = (ok && bRangeD(1, 60).contains(seconds)) ? seconds : 5;
    ok = false;
    n = bSettings->value("Fishing/panel_number", 1).toInt(&ok);
    vFishingPanelNumber = (ok && bRangeD(1, 10).contains(n)) ? n : 1;
    ok = false;
    n = bSettings->value("Fishing/main_panel_number", 1).toInt(&ok);
    vMainPanelNumber = (ok && bRangeD(1, 10).contains(n)) ? n : 1;
    ok = false;
    seconds = bSettings->value("Fishing/rest_time", 30).toInt(&ok);
    vRestTime = (ok && bRangeD(5, 300).contains(seconds)) ? seconds : 30;
    FishingKeyList list;
    int sz = bSettings->beginReadArray("Fishing/keys");
    foreach (int i, bRangeD(0, sz - 1))
    {
        bSettings->setArrayIndex(i);
        ok = false;
        int k = bSettings->value("key", 0).toInt(&ok);
        list << ((ok && bRangeD(0, 12).contains(k)) ? k : 0);
    }
    bSettings->endArray();
    if (list.size() == 9)
    {
        bool b = true;
        foreach (int k, list)
        {
            if (k > 0 && list.count(k) > 1)
            {
                b = false;
                break;
            }
        }
        if (b)
            vFishingKeyList = list;
    }
    QString fn = BDirTools::findResource("olympiad_message.png", BDirTools::UserOnly);
    QImage img = QImage(fn).createAlphaMask();
    if (img.width() > 300)
        img = img.copy(0, 0, 300, img.height());
    if (img.height() > 12)
        img = img.copy(0, 0, img.width(), 12);
    vOlympiadMessageMask = img;
    vCraftButtonPos = bSettings->value("Craft/button_pos", QPoint(-1, -1)).toPoint();
    ok = false;
    int x = bSettings->value("Craft/mp_regeneration", 10).toInt(&ok);
    vMpRegen = (ok && bRangeD(1, 100).contains(x)) ? x : 10;
    ok = false;
    x = bSettings->value("Craft/mp_consumption", 10).toInt(&ok);
    vMpConsumption = (ok && bRangeD(10, 500).contains(x)) ? x : 10;
    ok = false;
    seconds = bSettings->value("Craft/start_delay", 5).toInt(&ok);
    vCraftStartDelay = (ok && bRangeD(1, 60).contains(seconds)) ? seconds : 5;
}

void saveSettings()
{
    bSettings->setValue("System/game_dir", vGameDir);
    bSettings->setValue("System/timer_interval", vTimerInterval);
    bSettings->setValue("System/detection_delay", vDetectionDelay);
    bSettings->setValue("System/detect_window_id", vDetectWindowID);
    bSettings->setValue("Manor/olympiad_check_interval", vOlympiadCheckInterval);
    bSettings->setValue("Manor/timer_interval", vManorTimerInterval);
    bSettings->setValue("Manor/chat_row_count", vChatRowCount);
    bSettings->setValue("Manor/auto_start_enabled", vManorAutoStartEnabled);
    bSettings->setValue("Manor/auto_start_time", vManorAutoStartTime);
    bSettings->setValue("Manor/time_correction", vManorTimeCorrection);
    bSettings->setValue("Manor/button_pos", vManorButtonPos);
    bSettings->setValue("Fishing/equip_before_start", vFishingEquipBeforeStart);
    bSettings->setValue("Fishing/start_delay", vFishingStartDelay);
    bSettings->setValue("Fishing/panel_number", vFishingPanelNumber);
    bSettings->setValue("Fishing/main_panel_number", vMainPanelNumber);
    bSettings->setValue("Fishing/rest_time", vRestTime);
    bSettings->beginWriteArray("Fishing/keys", 9);
    foreach (int i, bRangeD(0, 8))
    {
        bSettings->setArrayIndex(i);
        bSettings->setValue("key", vFishingKeyList.at(i));
    }
    bSettings->endArray();
    bSettings->setValue("Craft/button_pos", vCraftButtonPos);
    bSettings->setValue("Craft/mp_regeneration", vMpRegen);
    bSettings->setValue("Craft/mp_consumption", vMpConsumption);
    bSettings->setValue("Craft/start_delay", vCraftStartDelay);
}

void setGameDir(const QString &dir)
{
    vGameDir = dir;
}

void setTimerInterval(int msecs)
{
    if (!bRangeD(1, 1000).contains(msecs))
        return;
    bool b = msecs != vTimerInterval;
    vTimerInterval = msecs;
    if (b)
        TimerWidget::resetTimerInterval();
}

void setDetectionDelay(int seconds)
{
    if (!bRangeD(1, 60).contains(seconds))
        return;
    vDetectionDelay = seconds;
}

void setDetectWindowID(bool b)
{
    vDetectWindowID = b;
}

void setOlympiadCheckInterval(int msecs)
{
    if (!bRangeD(1, 1000).contains(msecs))
        return;
    vOlympiadCheckInterval = msecs;
}

void setManorTimerInterval(int msecs)
{
    if (!bRangeD(1, 1000).contains(msecs))
        return;
    vManorTimerInterval = msecs;
    ManorWidget::resetTimerInterval();
}

void setChatRowCount(int n)
{
    if (!bRangeD(1, 28).contains(n))
        return;
    vChatRowCount = n;
}

void setManorAutoStartEnabled(bool enabled)
{
    if (enabled == vManorAutoStartEnabled)
        return;
    vManorAutoStartEnabled = enabled;
    ManorWidget::resetAutoStartEnabled();
}

void setManorAutoStartTime(const QTime &t)
{
    vManorAutoStartTime = t;
    ManorWidget::resetAutoStart();
}

void setManorTimeCorrection(int msecs)
{
    if (!bRangeD(-10000, 10000).contains(msecs))
        return;
    vManorTimeCorrection = msecs;
}

void setFishingEquipBeforeStart(bool b)
{
    vFishingEquipBeforeStart = b;
}

void setFishingStartDelay(int seconds)
{
    if (!bRangeD(1, 60).contains(seconds))
        return;
    vFishingStartDelay = seconds;
}

void setFishingPanelNumber(int n)
{
    if (!bRangeD(1, 10).contains(n))
        return;
    vFishingPanelNumber = n;
}

void setMainPanelNumber(int n)
{
    if (!bRangeD(1, 10).contains(n))
        return;
    vMainPanelNumber = n;
}

void setRestTime(int secs)
{
    if (!bRangeD(5, 300).contains(secs))
        return;
    vRestTime = secs;
}

void setFishingKeyList(const FishingKeyList &list)
{
    if (list.size() != 9)
        return;
    foreach (int k, list)
        if (!bRangeD(0, 12).contains(k) || (k > 0 && list.count(k) > 1))
            return;
    vFishingKeyList = list;
}

void setMpRegen(int x)
{
    if (!bRangeD(1, 100).contains(x))
        return;
    vMpRegen = x;
}

void setMpConsumption(int x)
{
    if (!bRangeD(10, 500).contains(x))
        return;
    vMpConsumption = x;
}

void setCraftStartDelay(int seconds)
{
    if (!bRangeD(1, 60).contains(seconds))
        return;
    vCraftStartDelay = seconds;
}

QString gameDir()
{
    return vGameDir;
}

int timerInterval()
{
    return vTimerInterval;
}

int detectionDelay()
{
    return vDetectionDelay;
}

bool detectWindowID()
{
    return vDetectWindowID;
}

int olympiadCheckInterval()
{
    return vOlympiadCheckInterval;
}

int manorTimerInterval()
{
    return vManorTimerInterval;
}

int chatRowCount()
{
    return vChatRowCount;
}

bool manorAutoStartEnabled()
{
    return vManorAutoStartEnabled;
}

QTime manorAutoStartTime()
{
    return vManorAutoStartTime;
}

int manorTimeCorrection()
{
    return vManorTimeCorrection;
}

QPoint manorButtonPos()
{
    return vManorButtonPos;
}

bool fishingEquipBeforeStart()
{
    return vFishingEquipBeforeStart;
}

int fishingStartDelay()
{
    return vFishingStartDelay;
}

int fishingPanelNumber()
{
    return vFishingPanelNumber;
}

int mainPanelNumber()
{
    return vMainPanelNumber;
}

int restTime()
{
    return vRestTime;
}

FishingKeyList fishingKeyList()
{
    return vFishingKeyList;
}

QString fishingKey(FishingAction a)
{
    if (a >= vFishingKeyList.size())
        return QString();
    int k = vFishingKeyList.at(a);
    if (!k)
        return QString();
    return "F" + QString::number(k);
}

const QImage *olympiadMessageMask()
{
    return &vOlympiadMessageMask;
}

QPoint craftButtonPos()
{
    return vCraftButtonPos;
}

int mpRegen()
{
    return vMpRegen;
}

int mpConsumption()
{
    return vMpConsumption;
}

int craftStartDelay()
{
    return vCraftStartDelay;
}

}
