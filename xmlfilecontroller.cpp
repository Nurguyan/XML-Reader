#include "xmlfilecontroller.h"
#include "xmlfileworker.h"

#include <QDebug>

XMLFileController::XMLFileController()
{
    XMLFileWorker *worker = new XMLFileWorker;
    worker->moveToThread(&worker_thread);

    connect(this, &XMLFileController::operate, worker, &XMLFileWorker::importFromFile);
    connect(this, &XMLFileController::stopWorker, worker, &XMLFileWorker::onCancel, Qt::DirectConnection); //direct connection in order to stop worker immediately
    connect(worker, &XMLFileWorker::sendImportedData, this, &XMLFileController::importCompleted);
    connect(worker, &XMLFileWorker::fileChanged, this, &XMLFileController::updateFileName);
    connect(worker, &XMLFileWorker::fileImportCompleted, this, &XMLFileController::fileImported);

    connect(worker, &XMLFileWorker::done, &worker_thread, &QThread::quit);
    connect(&worker_thread, &QThread::finished, worker, &QObject::deleteLater);
    worker_thread.start();
}

XMLFileController::~XMLFileController()
{
    worker_thread.quit();
    worker_thread.wait();
}

void XMLFileController::onCancelImport()
{
    emit stopWorker();
    worker_thread.quit();
    worker_thread.wait();
}



