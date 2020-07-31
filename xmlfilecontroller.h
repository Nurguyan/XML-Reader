#ifndef XMLFILECONTROLLER_H
#define XMLFILECONTROLLER_H

#include <QObject>
#include <QThread>
#include "texteditor.h"

class XMLFileController : public QObject
{
    Q_OBJECT
    QThread worker_thread;

public:
    XMLFileController();
    ~XMLFileController();

public slots:
    void onCancelImport();

signals:
    void operate(const QStringList &);                      //launch import
    void updateFileName(QString filename);                  //send new file's name to import dialog
    void importCompleted(const QList<TextEditor> &result);  //send result
    void fileImported(QString file, bool success);          //send filename and import result (success: true, fail: false)
    void stopWorker();
};

#endif // XMLFILECONTROLLER_H
