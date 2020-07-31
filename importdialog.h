#ifndef IMPORTDIALOG_H
#define IMPORTDIALOG_H
#include "ui_dialog.h"
#include <QDialog>
#include <QWidget>
#include "listmodel.h"

class ImportDialog : public QDialog
{
    Q_OBJECT

public:
    ImportDialog(QWidget *parent=nullptr);
    ~ImportDialog();

private:
    Ui::Dialog *ui;
    ListModel *model;

public slots:
    void onFileChanged(QString filename);               //triggered when file is being imported in XML Worker
    void onFileImported(QString file, bool success);    //triggered when file has been imported with either success or fail result in XML Worker
    void onSetProgressRange(int min, int max);          //triggered when progress needs to be changed
    void onFinished();                                  //triggered when user closes the dialog

signals:
    void cancelImport();
};

#endif // IMPORTDIALOG_H
