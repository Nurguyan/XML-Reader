#ifndef MODEL_H
#define MODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "texteditor.h"
#include "databaseworker.h"


class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TableModel(QObject *parent = nullptr);
    ~TableModel() override;

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    //Check if edited record is has unique primary key
    bool isUniquePrimaryKey(QString name);

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;


private:
    QList<TextEditor> text_editors;
    DatabaseWorker *db_worker;
    QThread *db_thread;

public slots:
    void addEntry(int row, const TextEditor text_editor);   //add record into database
    void removeEntry(const int row);                        //remove record from database
    void removeAllEntries();                                //delete all records from database

private slots:
    void updateModelData(const QList<TextEditor>);          //update model data

signals:
    void executeSqlQuery(QString);
    void updateData();                                      //load all current record from database

};

#endif // MODEL_H
