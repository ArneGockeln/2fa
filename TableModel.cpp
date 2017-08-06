//
// Created by Arne Gockeln on 28.07.17.
//

#include "TableModel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{

}

TableModel::TableModel(QList<ServiceItem> list, QObject *parent): QAbstractTableModel(parent) {
  m_listOfServiceItems = list;
}

int TableModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return m_listOfServiceItems.size();
}

int TableModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 3;
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
  if(!index.isValid()){
    return QVariant();
  }

  if(index.row() >= m_listOfServiceItems.size() || index.row() < 0){
    return QVariant();
  }

  if(role == Qt::DisplayRole){
    ServiceItem serviceItem = m_listOfServiceItems.at(index.row());

    if(index.column() == 0){
      return serviceItem.getName();
    } else if(index.column() == 1){
      return serviceItem.getSecret();
    } else if(index.column() == 2){
      return serviceItem.getInterval();
    }
  }

  return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
  if(role != Qt::DisplayRole){
    return QVariant();
  }

  if(orientation == Qt::Horizontal){
    switch(section){
      case 0:
        return tr("Name");
      case 1:
        return tr("Secret");
      case 2:
        return tr("Interval");
      default:
        return QVariant();
    }
  }

  return QVariant();
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index) {
  Q_UNUSED(index);

  beginInsertRows(QModelIndex(), position, position + rows - 1);

  for(int row = 0; row < rows; row++){
    ServiceItem serviceItem;
    m_listOfServiceItems.insert(position, serviceItem);
  }

  endInsertRows();
  return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index) {
  Q_UNUSED(index);
  beginRemoveRows(QModelIndex(), position, position + rows - 1);

  for(int row = 0; row < rows; row++){
    m_listOfServiceItems.removeAt(position);
  }

  emit(dataChanged(index, index));
  endRemoveRows();

  return true;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if(index.isValid() && role == Qt::EditRole){
    int row = index.row();

    ServiceItem serviceItem = m_listOfServiceItems.value(row);

    if(index.column() == 0){
      serviceItem.setName(value.toString());
    } else if(index.column() == 1) {
      serviceItem.setSecret(value.toString());
    } else if(index.column() == 2){
      serviceItem.setInterval(value.toInt());
    } else {
      return false;
    }

    m_listOfServiceItems.replace(row, serviceItem);
    emit(dataChanged(index, index));

    return true;
  }

  return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const {
  if(!index.isValid()){
    return Qt::ItemIsEnabled;
  }

  return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QList<ServiceItem> TableModel::getList(){
  return m_listOfServiceItems;
}
