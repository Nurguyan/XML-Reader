#include "listmodel.h"

ListModel::ListModel(QObject *parent) : QAbstractListModel(parent)
{

}

int ListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return list.count();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
             return QVariant();

         if (index.row() >= list.size())
             return QVariant();

         if (role == Qt::DisplayRole)
             return list.at(index.row());
         else
             return QVariant();
}

bool ListModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row) {
        list.insert(position, "");
    }
    endInsertRows();
    return true;
}

bool ListModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);
    for (int row = 0; row < rows; ++row) {
        list.removeAt(position);
    }

    endRemoveRows();
    return true;
}

Qt::ItemFlags ListModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
             return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool ListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        list.replace(index.row(), value.toString());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}
