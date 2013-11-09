#include "manorsettingstab.h"
#include "global.h"
#include "mainwindow.h"

#include <BAbstractSettingsTab>
#include <BPasswordWidget>
#include <BDirTools>

#include <QObject>
#include <QVariantMap>
#include <QString>
#include <QIcon>
#include <QCheckBox>
#include <QLineEdit>
#include <QFormLayout>
#include <QVariant>
#include <QByteArray>
#include <QSettings>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QComboBox>
#include <QToolButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QPixmap>

/*============================================================================
================================ ManorSettingsTab ============================
============================================================================*/

/*============================== Public constructors =======================*/

ManorSettingsTab::ManorSettingsTab()
{
    QFormLayout *flt = new QFormLayout(this);
      sboxOlympiadCheckInterval = new QSpinBox;
        sboxOlympiadCheckInterval->setMinimum(1);
        sboxOlympiadCheckInterval->setMaximum(1000);
        sboxOlympiadCheckInterval->setSingleStep(10);
        sboxOlympiadCheckInterval->setValue(Global::olympiadCheckInterval());
      flt->addRow(tr("Olympiad check interval (ms):", "lbl text"), sboxOlympiadCheckInterval);
      sboxTimerInterval = new QSpinBox;
        sboxTimerInterval->setMinimum(1);
        sboxTimerInterval->setMaximum(1000);
        sboxTimerInterval->setSingleStep(50);
        sboxTimerInterval->setValue(Global::manorTimerInterval());
      flt->addRow(tr("Timer interval (ms):", "lbl text"), sboxTimerInterval);
      sboxChatRowCount = new QSpinBox;
        sboxChatRowCount->setMinimum(1);
        sboxChatRowCount->setMaximum(28);
        sboxChatRowCount->setValue(Global::chatRowCount());
      flt->addRow(tr("Chat rows to check:", "lbl text"), sboxChatRowCount);
      QHBoxLayout *hlt = new QHBoxLayout;
        lblOlympiadMessageTemplate = new QLabel;
          lblOlympiadMessageTemplate->setPixmap(QPixmap::fromImage(Global::olympiadMessageMask()));
        hlt->addWidget(lblOlympiadMessageTemplate);
        btnGrabOlympiadMessage = new QPushButton(tr("Grab message", "btn text"));
          connect(btnGrabOlympiadMessage, SIGNAL(clicked()), this, SLOT(grabOlympiadMessage()));
        hlt->addWidget(btnGrabOlympiadMessage);
      flt->addRow(tr("Olympiad message:", "lbl text"), hlt);
      hlt = new QHBoxLayout;
        lblManorButtonPos = new QLabel;
          QPoint pos = Global::manorButtonPos();
          lblManorButtonPos->setText("<b>(" + QString::number(pos.x()) + "; " + QString::number(pos.y()) +")</b>");
        hlt->addWidget(lblManorButtonPos);
        btnSelectManorButton = new QPushButton(tr("Select position", "btn text"));
          connect(btnSelectManorButton, SIGNAL(clicked()), this, SLOT(selectManorButton()));
        hlt->addWidget(btnSelectManorButton);
      flt->addRow(tr("Manor button:", "lbl text"), hlt);
}

/*============================== Public methods ============================*/

QString ManorSettingsTab::title() const
{
    return tr("Manor", "title");
}

QIcon ManorSettingsTab::icon() const
{
    return BApplication::icon("harvester");
}

bool ManorSettingsTab::hasDefault() const
{
    return true;
}

bool ManorSettingsTab::restoreDefault()
{
    sboxOlympiadCheckInterval->setValue(10);
    sboxTimerInterval->setValue(50);
    sboxChatRowCount->setValue(4);
    return true;
}

bool ManorSettingsTab::saveSettings()
{
    Global::setOlympiadCheckInterval(sboxOlympiadCheckInterval->value());
    Global::setManorTimerInterval(sboxTimerInterval->value());
    Global::setChatRowCount(sboxChatRowCount->value());
    MainWindow::reloadInfo(MainWindow::ManorInfo);
    return true;
}

/*============================== Private slots =============================*/

void ManorSettingsTab::grabOlympiadMessage()
{
    QImage img = Global::grabOlympiadMessage();
    if (img.isNull())
        return;
    if (!Global::setOlympiadMessageTemplate(img))
        return;
    lblOlympiadMessageTemplate->setPixmap(QPixmap::fromImage(Global::olympiadMessageMask()));
    MainWindow::reloadInfo(MainWindow::OlympiadMessageInfo);
}

void ManorSettingsTab::selectManorButton()
{
    QPoint pos = Global::selectManorButtonPos(Global::manorButtonPos());
    if (pos.x() <= 0 || pos.y() <= 0)
        return;
    Global::setManorButtonPos(pos);
    lblManorButtonPos->setText("<b>(" + QString::number(pos.x()) + "; " + QString::number(pos.y()) +")</b>");
    MainWindow::reloadInfo(MainWindow::ManorButtonInfo);
}
