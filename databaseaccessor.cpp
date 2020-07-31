#include "databaseaccessor.h"
#include <QSqlQuery>

DatabaseAccessor::DatabaseAccessor()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName("localhost");
    db.setDatabaseName("database.db");

    if (db.open())
    {
        qDebug("Connected to database");
    }
    else
    {
        qDebug("Error occured in connection to database");
    }
}

DatabaseAccessor& DatabaseAccessor::getInstance()
{
    static DatabaseAccessor instance;
    return instance;
}

void DatabaseAccessor::executeSqlQuery(QString query)
{
    QSqlQuery sqlQuery(query, db);
}

DatabaseAccessor::~DatabaseAccessor()
{
    if (db.isOpen())
        db.close();
}

