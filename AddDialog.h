//
// Created by Arne Gockeln on 28.07.17.
//

#ifndef INC_2FA_ADDDIALOG_H
#define INC_2FA_ADDDIALOG_H


#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include "Presets.h"
#include <QCloseEvent>

class AddDialog: public QDialog {
  Q_OBJECT

public:
  AddDialog(QWidget *parent = 0);
  QLineEdit *m_nameText;
  QLineEdit *m_secretText;
  QLineEdit *m_validIntervalText;
  QComboBox *m_presetComboBox;
  bool validateAddEditDialog();

private:
  QLabel *m_nameLabel;
  QLabel *m_secretLabel;
  QLabel *m_validIntervalLabel;
  QLabel *m_presetLabel;
  QPushButton *m_okButton;
  QPushButton *m_cancelButton;

};


#endif //INC_2FA_ADDDIALOG_H
