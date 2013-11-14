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
#include <QTime>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
protected:
    void closeEvent(QCloseEvent *e);
private:
    void resetInterface(bool enabled);
private slots:
    void retranslateUi();
    void cboxToggled(bool b);
    void btnDetectClicked();
private:
    QSplitter *spltr;
      QTabWidget *twgtManor;
      //vlt
        QLabel *lblInfo;
        //hlt
          QCheckBox *cboxOnTop;
          QPushButton *btnSettings;
          QPushButton *btnDetect;
      QTabWidget *twgtBots;
        QWidget *wgtFishing;
        QWidget *wgtLeveling;
          //hlt
            QPushButton *btnLeveling;
          QPlainTextEdit *ptedtLeveling;
    //
    QMenu *mmnuFile;
      QAction *mactQuit;
    QMenu *mmnuEdit;
    QMenu *mmnuHelp;
};

#endif // MAINWINDOW_H
