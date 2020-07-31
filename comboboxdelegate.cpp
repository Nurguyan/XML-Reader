#include "comboboxdelegate.h"
#include <QComboBox>
#include <QDebug>
#include <QStyleOptionComboBox>

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}

QWidget *ComboBoxDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);
    QComboBox* comboBox = new QComboBox(parent);
    comboBox->addItem(QString("true"));
    comboBox->addItem(QString("false"));
    return comboBox;
}

void ComboBoxDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // update model widget
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    Q_ASSERT(comboBox);
    const QString currentText = index.data(Qt::EditRole).toString();
    const int cbIndex = comboBox->findText(currentText);
    // if it is valid, adjust the combobox
    if (cbIndex >= 0)
        comboBox->setCurrentIndex(cbIndex);
}

void ComboBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // store edited model data to model
    QComboBox* comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();
    model->setData(index, value, Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}


