#ifndef DATABASEACCESSOR_H
#define DATABASEACCESSOR_H
#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <memory>

//database singleton class
class DatabaseAccessor: public QObject
{
    Q_OBJECT

public:
    static DatabaseAccessor& getInstance();

public slots:
    void executeSqlQuery(QString);

private:
    QSqlDatabase db;

    DatabaseAccessor();
    ~DatabaseAccessor();
    DatabaseAccessor(const DatabaseAccessor&);
    DatabaseAccessor& operator=(const DatabaseAccessor&);
};

#endif // DATABASEACCESSOR_H
