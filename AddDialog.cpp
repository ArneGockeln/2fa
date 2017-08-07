//
// Created by Arne Gockeln on 28.07.17.
//

#include <QGridLayout>
#include <QFormLayout>
#include <QMessageBox>
#include "AddDialog.h"

AddDialog::AddDialog(QWidget *parent)
  : QDialog(parent)
{
  m_nameLabel = new QLabel(tr("Name"));
  m_secretLabel = new QLabel(tr("Secret"));
  m_validIntervalLabel = new QLabel(tr("Interval in seconds"));
  m_presetLabel = new QLabel(tr("Interval Preset"));
  m_okButton = new QPushButton(tr("OK"));
  m_cancelButton = new QPushButton(tr("Cancel"));

  m_nameText = new QLineEdit;
  m_secretText = new QLineEdit;
  m_validIntervalText = new QLineEdit;
  m_presetComboBox = new QComboBox;

  // fill presets
  m_presetComboBox->addItem(tr("Choose"));
  m_presetComboBox->addItem(tr("WordPress iThemes"), QVariant(TwoFA::Presets::WORDPRESS_ITHEMES));

  auto *formLayout = new QFormLayout;
  formLayout->addRow(m_nameLabel, m_nameText);
  formLayout->addRow(m_secretLabel, m_secretText);
  formLayout->addRow(m_presetLabel, m_presetComboBox);
  formLayout->addRow(m_validIntervalLabel, m_validIntervalText);

  auto *buttonLayout = new QHBoxLayout;
  buttonLayout->addWidget(m_okButton);
  buttonLayout->addWidget(m_cancelButton);

  formLayout->addRow(buttonLayout);

  auto *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(formLayout);
  setLayout(mainLayout);

  connect(m_okButton, &QAbstractButton::clicked, this, [=](){
    // validate edit form
    if(validateAddEditDialog()){
      accept();
    }
    // stay open until all valid
    return;
  }); //QDialog::accept
  connect(m_cancelButton, &QAbstractButton::clicked, this, &QDialog::reject);
  // assign preset defaults if combox changed
  connect(m_presetComboBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), [=](int index){
    if(m_presetComboBox->currentData() == TwoFA::Presets::WORDPRESS_ITHEMES){
      m_validIntervalText->setText("30");
    }
  });

  setWindowTitle(tr("Add"));
}

/**
 * Validate add edit entry dialog
 *
 * @return
 */
bool AddDialog::validateAddEditDialog(){
  bool result = true;

  // check name
  QString checkName = m_nameText->text().trimmed();
  if(checkName.isEmpty()){
    result = false;
    QMessageBox::information(this, tr("Info"), tr("Name is empty!"));
  }

  // check secret
  QString checkSecret = m_secretText->text().simplified().replace(" ", "").trimmed();
  if(checkSecret.isEmpty()){
    result = false;
    QMessageBox::information(this, tr("Info"), tr("Secret is empty!"));
  }

  // check secret string length
  if(checkSecret.length() % 8 != 0){
    result = false;
    QMessageBox::information(this, tr("Info"), tr("Secret string length is not divisible by 8!"));
  }

  // check interval
  QString checkInterval = m_validIntervalText->text().trimmed();
  if(checkInterval.isEmpty()){
    result = false;
    QMessageBox::information(this, tr("Info"), tr("Interval is empty!"));
  }

  // set valid values
  if(result){
    // set name to valid value
    m_nameText->setText(checkName);
    // set secret to valid value
    m_secretText->setText(checkSecret);
    // set interval to valid value
    m_validIntervalText->setText(checkInterval);
  }

  return result;
}