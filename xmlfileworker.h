#ifndef XMLFILEIMPORTER_H
#define XMLFILEIMPORTER_H
#include <QXmlStreamReader>
#include <QList>
#include <QMutex>
#include <QThread>
#include <QRunnable>
#include "texteditor.h"

class XMLFileWorker: public QObject
{
    Q_OBJECT
public:
    explicit XMLFileWorker();

private:
    mutable QMutex m_mutex; //protects m_cancel
    bool m_cancel;
    bool isCanceled();

public slots:
    void exportToFile(const QString &filename, const TextEditor &text_editor);
    void importFromFile(const QStringList &files);
    void onCancel();

signals:
    void fileChanged(QString filename);                     //send new file's name to import dialog
    void fileImportCompleted(QString file, bool success);   //param(bool success): false - import failed; true - import successful
    void sendImportedData(const QList<TextEditor> &result); //send result
    void done();                                            //notify thread to quit
};

#endif // XMLFILEIMPORTER_H
