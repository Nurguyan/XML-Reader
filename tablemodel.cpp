#include "tablemodel.h"
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QThread>

TableModel::TableModel(QObject *parent) : QAbstractTableModel (parent){}

TableModel::~TableModel(){}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
            switch (section) {
            case 0: return QString("Text editor");
            case 1: return QString("File formats");
            case 2: return QString("Encoding");
            case 3: return QString("IntelliSense");
            case 4: return QString("Plugins");
            case 5: return QString("Compiler");
            }
    }
    if (role == Qt::DisplayRole && orientation == Qt::Vertical) {
            return (section+1);
    }
    return QVariant();
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return text_editors.count();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 6;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto row = index.row();
    auto col = index.column();

    if (role == Qt::DisplayRole || role == Qt::EditRole){
        auto& tmp = text_editors.at(row);
        switch (col) {
            case 0: return tmp.getName();
            case 1: return tmp.getFormats();
            case 2: return tmp.getEncoding();
            case 3: if (tmp.hasIntellisense()) return QString("true"); else return QString("false");
            case 4: if (tmp.hasPlugins()) return QString("true"); else return QString("false");
            case 5: if (tmp.hasCompiler()) return QString("true"); else return QString("false");
        }     
    }
    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value && index.isValid() && role == Qt::EditRole) {
        if (value.toString().trimmed().isEmpty()) return false;
        auto row = index.row();
        auto col = index.column();
        auto tmp = text_editors.at(row);

        switch(col){
            case 0:
            if (isUniquePrimaryKey(value.toString()))
                tmp.setName(value.toString());
            else return false;
            break;
            case 1: tmp.setFormats(value.toString()); break;
            case 2: tmp.setEncoding(value.toString()); break;
            case 3: tmp.setIntellisense(value.toBool()); break;
            case 4: tmp.setPlugins(value.toBool()); break;
            case 5: tmp.setCompiles(value.toBool());  break;
        }       
        text_editors.replace(row, tmp);
        emit sendEditedData(row, tmp);
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }

    return false;
}

bool TableModel::isUniquePrimaryKey(QString name){
    if (name.isEmpty()) return false;
    for (auto& editor : text_editors){
        if (name.trimmed() == editor.getName()) return false;
    }
    return true;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();
    return true;
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        text_editors.removeAt(row);
    }
    endRemoveRows();
    return true;
}

void TableModel::addEntry(int row, const TextEditor text_editor)
{
    if (const auto& name = text_editor.getName(); !name.isEmpty() && isUniquePrimaryKey(name)) {   
        insertRows(row, 1, QModelIndex());
        text_editors.insert(row, text_editor);
        setData(index(row, 0, QModelIndex()), text_editor.getName(), Qt::EditRole);
        setData(index(row, 1, QModelIndex()), text_editor.getFormats(), Qt::EditRole);
        setData(index(row, 2, QModelIndex()), text_editor.getEncoding(), Qt::EditRole);
        setData(index(row, 3, QModelIndex()), text_editor.hasIntellisense(), Qt::EditRole);
        setData(index(row, 4, QModelIndex()), text_editor.hasPlugins(), Qt::EditRole);
        setData(index(row, 5, QModelIndex()), text_editor.hasCompiler(), Qt::EditRole);
    }
}

void TableModel::setTextEditorList(const QList<TextEditor>& list)
{
    text_editors = list;
    auto top_left = index(0,0);
    auto bottom_right = index(rowCount()-1,columnCount()-1);
    emit dataChanged(top_left, bottom_right);
    emit layoutChanged();
}



