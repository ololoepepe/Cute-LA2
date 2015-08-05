#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class QAction;
class QCheckBox;
class QCloseEvent;
class QLabel;
class QMenu;
class QPushButton;
class QSplitter;
class QTabWidget;
class QWidget;

#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QSplitter *spltr;
      QTabWidget *twgtManor;
        QWidget *wgtManor;
      //vlt
        QLabel *lblInfo;
        //hlt
          QCheckBox *cboxOnTop;
          QPushButton *btnSettings;
          QPushButton *btnDetect;
      QTabWidget *twgtBots;
        QWidget *wgtFishing;
        QWidget *wgtCraft;
    //
    QMenu *mmnuFile;
    QMenu *mmnuEdit;
    QMenu *mmnuHelp;
    bool mquit;
public:
    explicit MainWindow(QWidget *parent = 0);
public slots:
    void quit();
protected:
    void closeEvent(QCloseEvent *e);
private:
    void resetInterface(bool enabled);
private slots:
    void retranslateUi();
    void cboxToggled(bool b);
    void btnDetectClicked();
};

#endif // MAINWINDOW_H
