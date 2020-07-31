#include "tablemodel.h"
#include <QSql>
#include <QSqlQuery>
#include <QDebug>
#include <QThread>

TableModel::TableModel(QObject *parent) : QAbstractTableModel (parent){
    db_thread = new QThread();
    db_worker = new DatabaseWorker();
    //clean up after yourself
    connect(db_thread, &QThread::finished, db_thread, &QThread::deleteLater);
    connect(db_thread, &QThread::finished, db_worker, &QThread::deleteLater);
    //create connection once the thread starts
    connect(db_thread, &QThread::started, db_worker, &DatabaseWorker::createConnection);
    //send SQL queries from Model to the Database Worker
    connect(this, &TableModel::executeSqlQuery, db_worker, &DatabaseWorker::executeQuery, Qt::QueuedConnection);
    //get all records from database (i.e. send query "select * from text_editors")
    connect(this, &TableModel::updateData, db_worker, &DatabaseWorker::executeSelectAllQuery,Qt::QueuedConnection);

    db_worker->moveToThread(db_thread);
    db_thread->start();

    QString query("CREATE TABLE IF NOT EXISTS text_editors(text_editor TEXT NOT NULL, file_formats TEXT NOT NULL, "
                            "encoding TEXT NOT NULL, has_intellisense INTEGER DEFAULT 0, "
                            "has_plugins INTEGER DEFAULT 0, can_compile INTEGER DEFAULT 0)");
    emit executeSqlQuery(query);
    emit updateData();
}

TableModel::~TableModel()
{
   /* db_thread->quit();
    db_thread->wait();
    delete db_thread;
    delete db_worker;*/
}


void TableModel::updateModelData(const QList<TextEditor> list)
{
    text_editors = list;
    auto top_left = index(0,0);
    auto bottom_right = index(rowCount()-1,columnCount()-1);
    emit dataChanged(top_left, bottom_right);
    emit layoutChanged();
}

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
        auto update_query = QString("UPDATE text_editors SET text_editor='%1',file_formats='%2',encoding='%3',has_intellisense=%4,"
                                    "has_plugins=%5,can_compile=%6 WHERE text_editor = '%7';").arg(tmp.getName()).
                arg(tmp.getFormats()).arg(tmp.getEncoding()).arg(tmp.hasIntellisense()).arg(tmp.hasPlugins()).arg(tmp.hasCompiler()).
                arg(text_editors.at(row).getName());
        //!replace element in list only after creating update query
        text_editors.replace(row, tmp);
        emit executeSqlQuery(update_query);
        emit dataChanged(index, index);
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

        const QString query = QString("INSERT INTO text_editors(text_editor, file_formats, encoding, has_intellisense, has_plugins, can_compile) VALUES("
                      "'%1', '%2', '%3', %4, %5, %6)").arg(text_editor.getName()).arg(text_editor.getFormats()).arg(text_editor.getEncoding()).
                arg(int(text_editor.hasIntellisense())).arg(int(text_editor.hasPlugins())).arg(int(text_editor.hasCompiler()));
        emit executeSqlQuery(query);
    }
}

void TableModel::removeEntry(const int row)
{
    removeRows(row, 1, QModelIndex());
    const QString query = QString("DELETE FROM text_editors WHERE text_editor = '%1';").arg(data(index(row,0)).toString());
    emit executeSqlQuery(query);
}

void TableModel::removeAllEntries()
{
    removeRows(0, rowCount(), QModelIndex());
}

bool TableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
    {
        const QString query = QString("DELETE FROM text_editors WHERE text_editor = '%1';").arg(text_editors.at(row).getName());
        emit executeSqlQuery(query);
        text_editors.removeAt(row);
    }
    endRemoveRows();
    return true;
}

