#ifndef FISHINGWIDGET_H
#define FISHINGWIDGET_H

#include "global.h"

#include <BTranslation>

#include <QWidget>
#include <QImage>

class QPushButton;
class QPlainTextEdit;
class QEventLoop;

class FishingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FishingWidget(QWidget *parent = 0);
public:
    static void quitLoop();
private:
    bool testPecked(bool anyHp = true);
    bool testTarget();
    int getFishHp();
    void useReeling();
    void usePumping();
    void logFishing(const QString &text);
    bool wait(int msecs);
private slots:
    void retranslateUi();
    void btnClicked();
private:
    static FishingWidget *instance;
private:
    QPushButton *btn;
    QPlainTextEdit *ptedt;
    Global::FishingKeyList keys;
    QImage targetClose;
    bool active;
    bool fishing;
    bool mustExit;
    QEventLoop *loop;
    BTranslation trStart;
    BTranslation trStop;
};

#endif // FISHINGWIDGET_H
