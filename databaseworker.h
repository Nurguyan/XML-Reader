#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>
#include <QSqlDatabase>
#include "texteditor.h"

class DatabaseWorker : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseWorker(QObject *parent = nullptr, QString database_name = "database.db");
    ~DatabaseWorker();

private:
    QString db_name;
    QSqlDatabase db;


public slots:
    void createConnection();
    void executeQuery(const QString&);
    void executeSelectAllQuery();
    void onDataChanged(const int row, const TextEditor& edited_data);

signals:
    void sendSelectAllQueryResult(const QList<TextEditor>&);

};

#endif // DATABASEWORKER_H
