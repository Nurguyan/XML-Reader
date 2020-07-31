#include "databaseworker.h"
#include "databaseaccessor.h"
#include <QSqlQuery>
#include <QList>
#include <QDebug>
#include "texteditor.h"


DatabaseWorker::DatabaseWorker(QObject *parent) : QObject(parent){}

void DatabaseWorker::createConnection()
{
    DatabaseAccessor::getInstance();
}

void DatabaseWorker::executeQuery(const QString &queryString)
{
    //qDebug() << "SQL query: " << queryString;
    QSqlQuery query(QSqlDatabase::database());
    if (query.exec(queryString)){
        //qDebug() << "SQL query has been executed successfully.";
    }
    else {
        //qDebug() << "SQL query execution has failed.";
    }
}

void DatabaseWorker::executeSelectAllQuery()
{
    QString queryString("SELECT * FROM text_editors");
    //qDebug() << "SQL query: " << queryString;
    QSqlQuery query(QSqlDatabase::database());
    QList<TextEditor> text_editors;

    if (query.exec(queryString)){
        //qDebug() << "SQL query has been executed successfully.";
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
            connect(this, SIGNAL(sendSelectAllQueryResult(QList<TextEditor>)), QObject::sender(), SLOT(updateModelData(QList<TextEditor>)),Qt::BlockingQueuedConnection);
            emit sendSelectAllQueryResult(text_editors);
        }
    }
    else {
        //qDebug() << "SQL query execution has failed.";
    }
}


