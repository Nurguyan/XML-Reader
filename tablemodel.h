#ifndef MODEL_H
#define MODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "texteditor.h"


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
    void addEntry(int row, const TextEditor text_editor);
    void setTextEditorList(const QList<TextEditor>&);          //update model data
    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    QList<TextEditor> text_editors;

signals:
    void sendEditedData(const int row, const TextEditor& edited_data);
};

#endif // MODEL_H
