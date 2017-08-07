//
// Created by Arne Gockeln on 28.07.17.
//

#ifndef INC_2FA_ADDRESSWIDGET_H
#define INC_2FA_ADDRESSWIDGET_H


#include <QWidget>
#include <QItemSelection>
#include <QSortFilterProxyModel>
#include "TableModel.h"
#include <QTableView>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include "ServiceItem.h"
#include "AddDialog.h"

using namespace TwoFA;

class AddressWidget: public QWidget {
  Q_OBJECT

public:
  explicit AddressWidget(QWidget *t_parent = 0);
  void readFromFile();
  void writeToFile();

public slots:
  void showAddEntryDialog();
  void addEntry(ServiceItem& t_serviceItem);
  void editEntry();
  void removeEntry();
  TableModel *getTableModel();

signals:
  void selectionChanged(const QItemSelection &t_selected);

private:
  TableModel *m_table;
  QTableView *m_tableView;
  QSortFilterProxyModel *m_proxyModel;
  QPushButton *m_addButton;
  QPushButton *m_delButton;
};


#endif //INC_2FA_ADDRESSWIDGET_H
