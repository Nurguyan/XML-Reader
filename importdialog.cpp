#include "importdialog.h"

ImportDialog::ImportDialog(QWidget *parent) : QDialog(parent)
{
    ui = new Ui::Dialog();
    ui->setupUi(this);
    model = new ListModel(this);
    ui->lv_results->setModel(model);
    ui->pb_progress->setValue(0);
    connect(ui->buttonBox,&QDialogButtonBox::accepted,this, &ImportDialog::onFinished);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this, &ImportDialog::onFinished);
    connect(ui->buttonBox,&QDialogButtonBox::rejected,this, &ImportDialog::cancelImport);
}

ImportDialog::~ImportDialog()
{
    delete model;
    delete ui;
}

void ImportDialog::onSetProgressRange(int min, int max)
{
    ui->pb_progress->setRange(min, max);
}

void ImportDialog::onFinished()
{
    model->removeRows(0, model->rowCount());
    ui->pb_progress->reset();
}

void ImportDialog::onFileChanged(QString filename)
{
    ui->lb_progress->setText(QString("Importing (%1/%2): %3").arg(ui->pb_progress->value()).
                             arg(ui->pb_progress->maximum()).arg(filename));
}

void ImportDialog::onFileImported(QString file, bool success)
{
    auto position = 0;
    model->insertRows(position,1);
    ui->pb_progress->setValue(ui->pb_progress->value() + 1);
    if (success){
        model->setData(model->index(position,0), QString("Import successful: %1").arg(file), Qt::EditRole);
    }
    else {
        model->setData(model->index(position,0), QString("Import failed: %1").arg(file), Qt::EditRole);
    }
}




