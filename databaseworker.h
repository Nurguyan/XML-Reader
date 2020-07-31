#ifndef DATABASEWORKER_H
#define DATABASEWORKER_H

#include <QObject>
#include "texteditor.h"

class DatabaseWorker : public QObject
{
    Q_OBJECT

public:
    explicit DatabaseWorker(QObject *parent = nullptr);

public slots:
    void createConnection();
    void executeQuery(const QString &queryString);
    void executeSelectAllQuery();

signals:
    void sendSelectAllQueryResult(const QList<TextEditor>);

};

#endif // DATABASEWORKER_H
