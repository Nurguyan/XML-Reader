#include "databaseworker.h"
#include <QSqlQuery>
#include <QList>
#include <QDebug>
#include "texteditor.h"


DatabaseWorker::DatabaseWorker(QObject *parent, QString database_name) : QObject(parent), db_name(database_name) {}

DatabaseWorker::~DatabaseWorker()
{
    if (db.isOpen())
        db.close();
}

void DatabaseWorker::createConnection()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName(db_name);

    if (db.open())
    {
        qDebug("Connected to database");
        QString queryString("CREATE TABLE IF NOT EXISTS text_editors(text_editor TEXT PRIMARY KEY, file_formats TEXT NOT NULL, "
                                "encoding TEXT NOT NULL, has_intellisense INTEGER DEFAULT 0, "
                                "has_plugins INTEGER DEFAULT 0, can_compile INTEGER DEFAULT 0)");
        QSqlQuery query(queryString, db);
        query.exec();
    }
    else
    {
        qDebug("Error occured in connection to database");
    }
}

void DatabaseWorker::executeQuery(const QString &queryString)
{
    QSqlQuery query(queryString, db);
    query.exec();
}

void DatabaseWorker::executeSelectAllQuery()
{
    QString queryString("SELECT * FROM text_editors");

    QSqlQuery query(queryString, db);
    QList<TextEditor> text_editors;

    if (query.exec()){
        while (query.next())
        {
            TextEditor tmp;
            tmp.setName(query.value(0).toString());
            tmp.setFormats(query.value(1).toString());
            tmp.setEncoding(query.value(2).toString());
            tmp.setIntellisense(query.value(3).toBool());
            tmp.setPlugins(query.value(4).toBool());
            tmp.setCompiles(query.value(5).toBool());
            tmp.toString();
            text_editors.append(tmp);

            emit sendSelectAllQueryResult(text_editors);
        }
    }
}

void DatabaseWorker::onDataChanged(const int row, const TextEditor& edited_data)
{
    QSqlQuery query(db);

    query.prepare("INSERT OR REPLACE INTO text_editors(ROWID, text_editor,file_formats,encoding,has_intellisense,has_plugins,can_compile)"
                  "VALUES(?,?,?,?,?,?,?)");
    query.addBindValue(row);
    query.addBindValue(edited_data.getName());
    query.addBindValue(edited_data.getFormats());
    query.addBindValue(edited_data.getEncoding());
    query.addBindValue(edited_data.hasIntellisense());
    query.addBindValue(edited_data.hasPlugins());
    query.addBindValue(edited_data.hasCompiler());
    query.exec();
}


