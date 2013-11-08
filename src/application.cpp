#include "application.h"
#include "generalsettingstab.h"
#include "manorsettingstab.h"
#include "fishingsettingstab.h"

#include <BApplication>
#include <BSettingsDialog>
#include <BPasswordWidget>
#include <BAbstractSettingsTab>
#include <BLocaleComboBox>
#include <BDialog>

#include <QObject>
#include <QVariantMap>
#include <QByteArray>
#include <QList>
#include <QMessageBox>
#include <QApplication>
#include <QPushButton>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QSize>
#include <QPushButton>
#include <QFontComboBox>
#include <QSpinBox>
#include <QComboBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QCheckBox>
#include <QSettings>

#include <QDebug>

/*============================================================================
================================ Application =================================
============================================================================*/

/*============================== Public constructors =======================*/

Application::Application() :
    BApplication()
{
    //
}

Application::~Application()
{
    //
}

/*============================== Static public methods =====================*/

/*void Application::handleExternalRequest(const QStringList &args)
{
    if (!testAppInit())
        return;
    if (Global::multipleWindowsEnabled())
    {
        bApp->addMainWindow(args);
    }
    else
    {
        MainWindow *mw = !bApp->mmainWindows.isEmpty() ? bApp->mmainWindows.values().first() : 0;
        if (!mw)
            return;
    }
}

bool Application::showPasswordDialog(QWidget *parent)
{
    PasswordDialog pd(parent ? parent : mostSuitableWindow());
    if (pd.exec() != QDialog::Accepted)
        return false;
    Global::setPasswordSate(pd.passwordState());
    sClient->updateSettings();
    return true;
}

bool Application::showSettings(QWidget *parent)
{
    BDialog dlg(parent ? parent : mostSuitableWindow());
      dlg.setWindowTitle(tr("Updating account", "dlg windowTitle"));
      TUserWidget *uwgt = new TUserWidget(TUserWidget::UpdateMode);
        TUserInfo info(TUserInfo::UpdateContext);
        QStringList groups;
        sClient->getUserInfo(sClient->userId(), info, groups, parent);
        uwgt->setInfo(info);
        uwgt->setClabGroups(groups);
        uwgt->restorePasswordWidgetState(Global::passwordWidgetState());
        uwgt->restoreState(bSettings->value("UpdateUserDialog/user_widget_state").toByteArray());
        uwgt->setPassword(Global::password());
      dlg.setWidget(uwgt);
      dlg.addButton(QDialogButtonBox::Ok, SLOT(accept()));
      dlg.button(QDialogButtonBox::Ok)->setEnabled(uwgt->isValid());
      connect(uwgt, SIGNAL(validityChanged(bool)), dlg.button(QDialogButtonBox::Ok), SLOT(setEnabled(bool)));
      dlg.addButton(QDialogButtonBox::Cancel, SLOT(reject()));
      dlg.setMinimumSize(600, dlg.sizeHint().height());
      if (dlg.exec() != BDialog::Accepted)
          return false;
    info = uwgt->info();
    TOperationResult r = sClient->updateAccount(info, parent);
    if (r)
    {
        Global::setPassword(uwgt->password());
        Global::setPasswordWidgetSate(uwgt->savePasswordWidgetState());
        if (!sClient->updateSettings())
            sClient->reconnect();
        return true;
    }
    else
    {
        QMessageBox msg(parent);
        msg.setWindowTitle(tr("Changing account failed", "msgbox windowTitle"));
        msg.setIcon(QMessageBox::Critical);
        msg.setText(tr("The following error occured:", "msgbox text"));
        msg.setInformativeText(r.messageString());
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.exec();
        Global::setPasswordWidgetSate(uwgt->savePasswordWidgetState());
        return false;
    }
}

bool Application::showRegisterDialog(QWidget *parent)
{
    QDialog dlg(parent ? parent : mostSuitableWindow());
    dlg.setWindowTitle(tr("Registration", "dlg windowTitle"));
    QVBoxLayout *vlt = new QVBoxLayout(&dlg);
      TUserWidget *uwgt = new TUserWidget(TUserWidget::RegisterMode);
        uwgt->restorePasswordWidgetState(Global::passwordWidgetState());
      vlt->addWidget(uwgt);
      vlt->addStretch();
      QDialogButtonBox *dlgbbox = new QDialogButtonBox;
        dlgbbox->addButton(QDialogButtonBox::Ok);
        dlgbbox->button(QDialogButtonBox::Ok)->setEnabled(false);
        connect(uwgt, SIGNAL(validityChanged(bool)), dlgbbox->button(QDialogButtonBox::Ok), SLOT(setEnabled(bool)));
        connect(dlgbbox->button(QDialogButtonBox::Ok), SIGNAL(clicked()), &dlg, SLOT(accept()));
        dlgbbox->addButton(QDialogButtonBox::Cancel);
        connect(dlgbbox->button(QDialogButtonBox::Cancel), SIGNAL(clicked()), &dlg, SLOT(reject()));
      vlt->addWidget(dlgbbox);
      dlg.setMinimumWidth(700);
      dlg.setFixedHeight(dlg.sizeHint().height());
    while (dlg.exec() == QDialog::Accepted)
    {
        TUserInfo info = uwgt->info();
        TOperationResult r = Client::registerUser(info, dlg.parentWidget());
        if (r)
        {
            Global::setLogin(info.login());
            Global::setPassword(uwgt->password());
            Global::setPasswordWidgetSate(uwgt->savePasswordWidgetState());
            sClient->updateSettings();
            sClient->connectToServer();
            return true;
        }
        else
        {
            QMessageBox msg(dlg.parentWidget());
            msg.setWindowTitle(tr("Registration error", "msgbox windowTitle"));
            msg.setIcon(QMessageBox::Critical);
            msg.setText(tr("Failed to register due to the following error:", "msgbox text"));
            msg.setInformativeText(r.messageString());
            msg.setStandardButtons(QMessageBox::Ok);
            msg.setDefaultButton(QMessageBox::Ok);
            msg.exec();
        }
    }
    Global::setPasswordWidgetSate(uwgt->savePasswordWidgetState());
    return false;
}*/

/*============================== Protected methods =========================*/

QList<BAbstractSettingsTab *> Application::createSettingsTabs() const
{
    QList<BAbstractSettingsTab *> list;
    list << new GeneralSettingsTab;
    list << new ManorSettingsTab;
    list << new FishingSettingsTab;
    return list;
}

/*============================== Static private methods ====================*/

/*bool Application::testAppInit()
{
    return bTest(bApp, "Application", "There must be an Application instance");
}*/

/*============================== Private methods ===========================*/

/*void Application::addMainWindow(const QStringList &)
{
    MainWindow *mw = new MainWindow;
    mw->setAttribute(Qt::WA_DeleteOnClose, true);
    connect(mw, SIGNAL(destroyed(QObject *)), this, SLOT(mainWindowDestroyed(QObject *)));
    mmainWindows.insert(mw, mw);
    mw->show();
}*/

/*============================== Private slots =============================*/

/*void Application::mainWindowDestroyed(QObject *obj)
{
    mmainWindows.remove(obj);
}*/
