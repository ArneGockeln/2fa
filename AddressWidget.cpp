//
// Created by Arne Gockeln on 28.07.17.
//

#include <QMessageBox>
#include <QVBoxLayout>
#include "AddressWidget.h"
#include "AddDialog.h"
#include "Storage.h"
#include "MainWindow.h"
#include <QHeaderView>
#include <QDebug>
#include <QDir>

AddressWidget::AddressWidget(QWidget *t_parent)
  : QWidget(t_parent)
{
  m_table = new TableModel(this);
  readFromFile();

  m_proxyModel = new QSortFilterProxyModel(this);
  m_proxyModel->setSourceModel(m_table);
  m_proxyModel->setFilterKeyColumn(0);

  m_tableView = new QTableView;
  m_tableView->setModel(m_proxyModel);

  m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_tableView->horizontalHeader()->setStretchLastSection(true);
  m_tableView->horizontalHeader()->hide();
  m_tableView->verticalHeader()->hide();
  m_tableView->horizontalHeader()->setMinimumSectionSize(1);
  m_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
  m_tableView->setSortingEnabled(true);

  auto addButton = new QPushButton(tr("+"));
  auto delButton = new QPushButton(tr("-"));

  auto *buttonLayout = new QHBoxLayout();
  buttonLayout->addWidget(addButton);
  buttonLayout->addWidget(delButton);

  auto *layout = new QVBoxLayout();
  layout->addWidget(m_tableView);
  layout->addLayout(buttonLayout);
  setLayout(layout);

  connect(m_tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AddressWidget::selectionChanged);
  connect(m_tableView, &QTableView::doubleClicked, this, &AddressWidget::editEntry);

  connect(addButton, &QAbstractButton::clicked, this, &AddressWidget::showAddEntryDialog);
  connect(delButton, &QAbstractButton::clicked, this, &AddressWidget::removeEntry);

}

/**
 * Add Entry Dialog
 */
void AddressWidget::showAddEntryDialog() {
  AddDialog addDialog;

  if(addDialog.exec() == QDialog::Accepted)
  {
    if(addDialog.validateAddEditDialog()){
      ServiceItem serviceItem;
      serviceItem.setName(addDialog.m_nameText->text());
      serviceItem.setSecret(addDialog.m_secretText->text());
      serviceItem.setInterval(addDialog.m_validIntervalText->text().toInt());

      addEntry(serviceItem);
    }
  }
}

/**
 * Add entry to tableView and write table model to file
 *
 * @param t_name
 * @param t_secret
 * @param t_validInterval
 */
void AddressWidget::addEntry(ServiceItem& t_serviceItem){
  QList<ServiceItem> list = m_table->getList();

  if(!list.contains(t_serviceItem)){
    m_table->insertRows(0, 1, QModelIndex());

    QModelIndex index = m_table->index(0, 0, QModelIndex());
    m_table->setData(index, t_serviceItem.getName(), Qt::EditRole);
    index = m_table->index(0, 1, QModelIndex());
    m_table->setData(index, t_serviceItem.getSecret(), Qt::EditRole);
    index = m_table->index(0, 2, QModelIndex());
    m_table->setData(index, t_serviceItem.getInterval(), Qt::EditRole);

    writeToFile();
  } else {
    QMessageBox::information(this, tr("Duplicate Name"), tr("The name \"%1\" already exists.").arg(t_serviceItem.getName()));
  }
}

/**
 * Edit entry and show edit dialog
 */
void AddressWidget::editEntry() {
  auto *temp = m_tableView;
  auto *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
  QItemSelectionModel *selectionModel = temp->selectionModel();

  QModelIndexList indexes = selectionModel->selectedRows();
  int row = -1;
  ServiceItem serviceItem;

  foreach(QModelIndex index, indexes){
    row = proxy->mapToSource(index).row();
    QModelIndex nameIndex = m_table->index(row, 0, QModelIndex());
    QVariant varName = m_table->data(nameIndex, Qt::DisplayRole);
    serviceItem.setName( varName.toString() );

    QModelIndex secretIndex = m_table->index(row, 1, QModelIndex());
    QVariant varSecret = m_table->data(secretIndex, Qt::DisplayRole);
    serviceItem.setSecret( varSecret.toString() );

    QModelIndex validIntervalIndex = m_table->index(row, 2, QModelIndex());
    QVariant varValidInterval = m_table->data(validIntervalIndex, Qt::DisplayRole);
    serviceItem.setInterval( varValidInterval.toInt() );
  }

  AddDialog addDialog;
  addDialog.setWindowTitle(tr("Edit"));
  addDialog.m_nameText->setText(serviceItem.getName());
  addDialog.m_secretText->setText(serviceItem.getSecret());
  addDialog.m_validIntervalText->setText(QString::number(serviceItem.getInterval()));

  if(addDialog.exec() == QDialog::Accepted){
    if(addDialog.validateAddEditDialog()){
      // name text
      QString newName = addDialog.m_nameText->text();
      if(newName != serviceItem.getName()){
        QModelIndex index = m_table->index(row, 0, QModelIndex());
        m_table->setData(index, newName, Qt::EditRole);
      }

      // secret text
      QString newSecret = addDialog.m_secretText->text();
      if(newSecret != serviceItem.getSecret()){
        QModelIndex index = m_table->index(row, 1, QModelIndex());
        m_table->setData(index, newSecret, Qt::EditRole);
      }

      // valid interval text
      QString newValidInterval = addDialog.m_validIntervalText->text();
      if(newValidInterval != serviceItem.getInterval()){
        QModelIndex index = m_table->index(row, 2, QModelIndex());
        m_table->setData(index, newValidInterval, Qt::EditRole);
      }

      // write to file
      writeToFile();
    }
  }
}

void AddressWidget::removeEntry() {
  auto *temp = m_tableView;
  auto *proxy = static_cast<QSortFilterProxyModel*>(temp->model());
  auto *selectionModel = temp->selectionModel();

  QModelIndexList indexes = selectionModel->selectedRows();

  foreach(QModelIndex index, indexes){
    int row = proxy->mapToSource(index).row();
    m_table->removeRows(row, 1, QModelIndex());
  }

  writeToFile();
}

/**
 * Save listview entries to storage
 */
void AddressWidget::writeToFile() {
  Storage::getInstance()->write(m_table);
}

/**
 * Read from storage file and populate listview
 */
void AddressWidget::readFromFile() {
  Storage::getInstance()->read(m_table);
}

TableModel *AddressWidget::getTableModel() {
  return m_table;
}