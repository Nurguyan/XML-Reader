#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressDialog>
#include "databaseworker.h"
#include "tablemodel.h"
#include "comboboxdelegate.h"
#include "xmlfileworker.h"
#include "xmlfilecontroller.h"
#include "importdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TableModel *model;                      //table view model
    ComboBoxDelegate *combo_box_delegate;   //delegate for table columns with boolean type: true/false
    XMLFileController *xmlcontroller;       //XML Controller for XML Worker (import/export)
    ImportDialog *import_dlg;               //dialog for import progress

    void importFiles(QStringList files);

private slots:
    void onImportFilesBtClick();            //import file(s) menu item clicked
    void onImportDirBtClick();              //import dir menu item clicked
    void onClearBtClick();                  //clear button clicked
    void onCustomMenuRequested(QPoint pos); //context menu on right click on tableview
    void onEditRowAction();                 //edit item clicked in the context menu
    void onRemoveRowsAction();              //delete item clicked in the context menu
    void onExportRowAction();               //export item clicked in the context menu
    void onImportCompleted(const QList<TextEditor> &result);    //receive imported data from xmlworker

signals:
    void setImportDlgProgressRange(int min, int max);   //set progress range of import dialog
};

#endif // MAINWINDOW_H
