#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFuture>
#include <QtConcurrent/QtConcurrentRun>
#include <QDebug>
#include <QAction>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //GUI
    connect(ui->actionOpen_File, &QAction::triggered, this, &MainWindow::onImportFilesBtClick); //connecting import file event to its slot
    connect(ui->actionOpen_Folder, &QAction::triggered, this, &MainWindow::onImportDirBtClick); //connecting import directory event to its slot
    connect(ui->actionClear_database,  &QAction::triggered, this, &MainWindow::onClearBtClick); //connecting clear DB to its slot
    connect(ui->actionExit, &QAction::triggered, QCoreApplication::instance(), &QCoreApplication::quit);

    //TableView
    model = new TableModel(this);
    ui->tbv_text_editors->setModel(model);
    combo_box_delegate = new ComboBoxDelegate(ui->tbv_text_editors);
    ui->tbv_text_editors->setItemDelegateForColumn(3, combo_box_delegate);
    ui->tbv_text_editors->setItemDelegateForColumn(4, combo_box_delegate);
    ui->tbv_text_editors->setItemDelegateForColumn(5, combo_box_delegate);
    ui->tbv_text_editors->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tbv_text_editors, &QTableView::customContextMenuRequested, this, &MainWindow::onCustomMenuRequested);
    ui->tbv_text_editors->show();

    //Database
    db_thread = new QThread();
    db_worker = new DatabaseWorker();
    connect(db_thread, &QThread::finished, db_thread, &QThread::deleteLater); //clean up after yourself
    connect(db_thread, &QThread::finished, db_worker, &QObject::deleteLater); //clean up after yourself
    connect(db_thread, &QThread::started, db_worker, &DatabaseWorker::createConnection);//create connection once the thread starts
    connect(this, &MainWindow::executeSqlQuery, db_worker, &DatabaseWorker::executeQuery, Qt::QueuedConnection); //send SQL queries from Model to the Database Worker
    connect(this, &MainWindow::updateData, db_worker, &DatabaseWorker::executeSelectAllQuery,Qt::QueuedConnection); //get all records from database
    connect(db_worker, &DatabaseWorker::sendSelectAllQueryResult, model, &TableModel::setTextEditorList, Qt::BlockingQueuedConnection);
    connect(model, &TableModel::sendEditedData, db_worker, &DatabaseWorker::onDataChanged); //update database when table model is edited

    db_worker->moveToThread(db_thread);
    db_thread->start();

    emit updateData();
}

void MainWindow::importFiles(QStringList files)
{
    xmlcontroller = new XMLFileController();
    connect(xmlcontroller, &XMLFileController::importCompleted, this, &MainWindow::onImportCompleted);

    import_dlg = new ImportDialog(this);
    connect(this, &MainWindow::setImportDlgProgressRange, import_dlg, &ImportDialog::onSetProgressRange);
    connect(xmlcontroller, &XMLFileController::updateFileName, import_dlg, &ImportDialog::onFileChanged);
    connect(xmlcontroller, &XMLFileController::fileImported, import_dlg, &ImportDialog::onFileImported);
    connect(import_dlg, &ImportDialog::cancelImport, xmlcontroller, &XMLFileController::onCancelImport);

    import_dlg->show();
    emit setImportDlgProgressRange(0, files.count());
    xmlcontroller->operate(files);
}

void MainWindow::onImportFilesBtClick()
{
    QFileDialog file_dlg(this, tr("Import XML files from directory"), nullptr, "*.xml");
    file_dlg.setAcceptMode(QFileDialog::AcceptOpen);
    file_dlg.setFileMode(QFileDialog::ExistingFiles);
    if (file_dlg.exec() == QFileDialog::Accepted)
    {
        QStringList files = file_dlg.selectedFiles();
        if (files.empty())
            return;
        importFiles(files);
    }
}

void MainWindow::onImportDirBtClick()
{
    QFileDialog dlg(this, tr("Import XML files from directory"));
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    dlg.setFileMode(QFileDialog::Directory);
    if (dlg.exec() == QFileDialog::Accepted)
    {
        QDir directory = dlg.selectedFiles().at(0);
        if (directory.absolutePath().isEmpty() || directory.absolutePath().isNull())
            return;

        QStringList files = directory.entryList(QStringList() << "*.xml",QDir::Files);
        if (files.empty())
            return;
        for (auto& filename : files){
            filename = directory.filePath(filename);//concatenate dir name and file name
        }
        importFiles(files);
    }
}

void MainWindow::onClearBtClick()
{
    QMessageBox msgBox;
    msgBox.setText("This action will remove all records from database.");
    msgBox.setInformativeText("Are you sure you want to delete all records?");
    msgBox.setStandardButtons(QMessageBox::Yes |  QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Yes);
    int ret = msgBox.exec();

    if (ret == QMessageBox::Yes)
    {
        model->removeRows(0, model->rowCount());
        const QString query = QString("DELETE FROM text_editors");
        emit executeSqlQuery(query);
    }
}

void MainWindow::onCustomMenuRequested(QPoint pos){
    QModelIndex index=ui->tbv_text_editors->indexAt(pos);
    QMenu *menu=new QMenu(this);
    QAction *edit = new QAction("Edit");
    edit->setData(index);
    QAction *remove_row = new QAction("Delete row");
    remove_row->setData(index);
    QAction *export_row = new QAction("Export to XML file...");
    export_row->setData(index);
    menu->addAction(edit);
    menu->addSeparator();
    menu->addAction(remove_row);
    menu->addSeparator();
    menu->addAction(export_row);
    menu->popup(ui->tbv_text_editors->viewport()->mapToGlobal(pos));
    connect(edit, &QAction::triggered, this, &MainWindow::onEditRowAction);
    connect(remove_row, &QAction::triggered, this, &MainWindow::onRemoveRowAction);
    connect(export_row, &QAction::triggered, this, &MainWindow::onExportRowAction);
}

void MainWindow::onEditRowAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    auto index = action->data().toModelIndex();
    ui->tbv_text_editors->edit(index);
}

void MainWindow::onRemoveRowAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    auto index = action->data().toModelIndex();
    QString name = model->data(model->index(index.row(),0)).toString();
    model->removeRows(index.row(),1,QModelIndex());
    const QString query = QString("DELETE FROM text_editors WHERE text_editor = '%1';").arg(name);
    emit executeSqlQuery(query);
}

void MainWindow::onExportRowAction()
{
    QAction* action = qobject_cast<QAction*>(sender());
    auto row = action->data().toModelIndex().row();
    auto name = model->data(model->index(row,0)).toString();
    auto formats = model->data(model->index(row,1)).toString();
    auto encoding = model->data(model->index(row,2)).toString();
    bool intellisense = !model->data(model->index(row,3)).toString().compare("true");
    bool plugins = !model->data(model->index(row,4)).toString().compare("true");
    bool compiler = !model->data(model->index(row,5)).toString().compare("true");
    TextEditor text_editor(name, formats, encoding, intellisense, plugins, compiler);

    QFileDialog dlg(this, nullptr, "*.xml");
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.selectFile(name + ".xml");
    if (dlg.exec() == QFileDialog::Accepted)
    {
        QString filename = dlg.selectedFiles().at(0);
        if (filename.isEmpty() || filename.isNull())
            return;

        XMLFileWorker xml_worker;
        QFuture<void> future = QtConcurrent::run(&xml_worker, &XMLFileWorker::exportToFile, filename, text_editor);
    }
}

void MainWindow::onImportCompleted(const QList<TextEditor> &result)
{
    for (auto& text_editor : result){
        model->addEntry(model->rowCount(), text_editor);
        const QString query = QString("INSERT INTO text_editors(text_editor, file_formats, encoding, has_intellisense, has_plugins, can_compile) VALUES("
                      "'%1', '%2', '%3', %4, %5, %6)").arg(text_editor.getName()).arg(text_editor.getFormats()).arg(text_editor.getEncoding()).
                arg(int(text_editor.hasIntellisense())).arg(int(text_editor.hasPlugins())).arg(int(text_editor.hasCompiler()));
        emit executeSqlQuery(query);
    }
}

MainWindow::~MainWindow()
{
    db_thread->quit();
    db_thread->wait();
    delete import_dlg;
    delete xmlcontroller;
    delete model;
    delete combo_box_delegate;
    delete ui;
}
