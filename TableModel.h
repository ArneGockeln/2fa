//
// Created by Arne Gockeln on 28.07.17.
//

#ifndef INC_2FA_TABLEMODEL_H
#define INC_2FA_TABLEMODEL_H


#include <QAbstractTableModel>
#include "ServiceItem.h"
using namespace TwoFA;

class TableModel: public QAbstractTableModel {
  Q_OBJECT

public:
  TableModel(QObject *parent = 0);
  TableModel(QList<ServiceItem> list, QObject *parent = 0);

  int rowCount(const QModelIndex &parent) const override ;
  int columnCount(const QModelIndex &parent) const override ;
  QVariant data(const QModelIndex &index, int role) const override ;
  QVariant headerData(int section, Qt::Orientation orientation, int role) const override ;
  Qt::ItemFlags flags(const QModelIndex &index) const override ;
  bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override ;
  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override ;
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override ;
  QList<ServiceItem> getList();

private:
  QList<ServiceItem> m_listOfServiceItems;
};


#endif //INC_2FA_TABLEMODEL_H
