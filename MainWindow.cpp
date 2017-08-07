//
// Created by Arne Gockeln on 28.07.17.
//

#include <QVBoxLayout>
#include <QMessageBox>
#include <QApplication>
#include "MainWindow.h"
#include "Storage.h"
#include "Auth.h"
#include <QClipboard>

using namespace TwoFA;

MainWindow::MainWindow() {
  m_addressWidget = new AddressWidget(this);
  // connect tablemodel changed signal with rebuilding trayicon
  connect(m_addressWidget->getTableModel(), SIGNAL(dataChanged(QModelIndex, QModelIndex)),
          this, SLOT(onUpdateModel()));

  // check storage
  Storage::getInstance()->checkStorage();

  auto *mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_addressWidget);
  setLayout(mainLayout);

  createActions();
  createTrayIcon();

  setWindowTitle(tr("2FA"));
  setMinimumSize(480, 320);
  setMaximumSize(480, 320);
  resize(480, 320);
  setWindowFlags(Qt::Dialog | Qt::WindowStaysOnTopHint);
}

/**
 * Create actions
 */
void MainWindow::createActions() {
  m_aboutAction = new QAction(tr("&About"), this);
  connect(m_aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);

  m_restoreAction = new QAction(tr("&Manage Sites"), this);
  connect(m_restoreAction, &QAction::triggered, this, &QWidget::showNormal);

  m_quitAction = new QAction(tr("&Quit"), this);
  connect(m_quitAction, &QAction::triggered, qApp, &QCoreApplication::quit);
}

/**
 * Hook into setVisible method
 *
 * @param t_visible
 */
void MainWindow::setVisible(bool t_visible) {
  m_restoreAction->setEnabled(isMaximized() || !t_visible);
  QWidget::setVisible(t_visible);
}

/**
 * ignore close button, hide form
 *
 * @param t_event
 */
void MainWindow::closeEvent(QCloseEvent *t_event) {
#ifdef Q_OS_OSX
  if(!t_event->spontaneous() || !isVisible()){
    return;
  }
#endif

  if(m_trayIcon->isVisible()){
    hide();
    t_event->ignore();
  }
}

/**
 * SLOT: updates the tray icon after adding or removing
 * entries to the tablemodel
 */
void MainWindow::onUpdateModel(){
  addActionList();
}

/**
 * Creates the traymenu with icon and sites
 */
void MainWindow::createTrayIcon() {

  m_trayIconMenu = new QMenu(this);

  // add sites with countdown
  addActionList();

  connect(m_trayIconMenu, SIGNAL(triggered(QAction*)), this, SLOT(copyToClipboard(QAction*)));

  m_trayIcon = new QSystemTrayIcon(this);
  m_trayIcon->setContextMenu(m_trayIconMenu);

  QIcon icon(":/images/lockicon.png");
#ifdef Q_OS_OSX
  icon.setIsMask(true);
#endif
  setWindowIcon(icon);
  m_trayIcon->setIcon(icon);
  m_trayIcon->show();
}

void MainWindow::showAboutDialog(){
  QMessageBox::about(0, tr("About Menu"), tr("Idea and coding by Arne Gockeln (c)2017, https://arnegockeln.com"));
}

/**
 * Populate TrayMenu with action list from
 * secrets file
 */
void MainWindow::addActionList(){
  auto *tableModel = m_addressWidget->getTableModel();

  QList<ServiceItem> serviceList = tableModel->getList();

  m_trayIconMenu->clear();

  if(!serviceList.isEmpty()){
    for(int i = 0; i < serviceList.size(); i++){
      auto serviceItem = serviceList.at(i);

      //QString actionName = QString("%2").arg((serviceItem.getInterval() - (time(nullptr) % serviceItem.getInterval()))).arg(serviceItem.getName());
      QString actionName = serviceItem.getName();

      auto *temp = new QAction(actionName, this);

      QString dataString = serviceItem.getSecret();
      dataString.append(":");
      dataString.append(QString::number(serviceItem.getInterval()));

      QVariant data(dataString);

      temp->setData(data);
      m_trayIconMenu->addAction(temp);
    }

    m_trayIconMenu->addSeparator();
  }

  m_trayIconMenu->addAction(m_restoreAction);
  m_trayIconMenu->addAction(m_aboutAction);
  m_trayIconMenu->addSeparator();
  m_trayIconMenu->addAction(m_quitAction);
}

/**
 * Handle action from tray clicked
 * @param t_action
 */
void MainWindow::copyToClipboard(QAction *t_action){

  QString actionData = t_action->data().toString();

  // this happens, if a action is fired, that is no site entry
  if(actionData.isEmpty()){
    return;
  }

  try {
    // split string
    QStringList actionDataList = actionData.split(":");
    QString secret = actionDataList.at(0);
    int interval = actionDataList.at(1).toInt();

    // Generate Token
    uint32_t token = Auth::getInstance()->generateToken(secret, interval);
    char strToken[16];
    sprintf(strToken, "%u", token);
    QString qToken(strToken);

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(qToken.trimmed());
  } catch(std::invalid_argument& argument){
    QMessageBox::critical(Q_NULLPTR, tr("Error"), argument.what());
  } catch(std::exception& exception){
    QMessageBox::critical(Q_NULLPTR, tr("Error"), exception.what());
  }
}