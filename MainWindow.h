//
// Created by Arne Gockeln on 28.07.17.
//

#ifndef INC_2FA_MAINWINDOW_H
#define INC_2FA_MAINWINDOW_H


#include <QWidget>
#include "AddressWidget.h"
#include <QAction>
#include <QMenu>
#include <QSystemTrayIcon>
#include <QCloseEvent>

class MainWindow: public QWidget {
  Q_OBJECT
private:
  AddressWidget *m_addressWidget;
  QMenu *m_trayIconMenu;
  QAction *m_aboutAction;
  QAction *m_restoreAction;
  QAction *m_quitAction;
  QSystemTrayIcon *m_trayIcon;
  void createActions();
  void createTrayIcon();
  void setVisible(bool t_visible) override;
  void addActionList();

protected:
  void closeEvent(QCloseEvent *t_event) override;

private slots:
  void showAboutDialog();
  void copyToClipboard(QAction *t_action);

public slots:
  void onUpdateModel();

public:
  MainWindow();


};


#endif //INC_2FA_MAINWINDOW_H
