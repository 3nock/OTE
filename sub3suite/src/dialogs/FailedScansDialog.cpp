/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "FailedScansDialog.h"
#include "ui_FailedScansDialog.h"

#include <QMenu>
#include <QAction>
#include <QClipboard>


FailedScansDialog::FailedScansDialog(QWidget *parent, QMap<QString,QString> targets) :
    QDialog(parent),
    ui(new Ui::FailedScansDialog),
    m_model(new QStandardItemModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/error.png"));

    // initializing the model
    m_model->setColumnCount(2);
    m_model->setHorizontalHeaderLabels({" Target", " Error"});

    // appending the targets and their errors on the model
    QStringList targetList = targets.keys();
    foreach(const QString &target, targetList)
        m_model->appendRow({new QStandardItem(target), new QStandardItem(targets.value(target))});

    // model to the tableview
    ui->tableView->setModel(m_model);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

}
FailedScansDialog::~FailedScansDialog(){
    delete m_model;
    delete ui;
}

void FailedScansDialog::on_buttonReScan_clicked(){
    QQueue<QString> targets;
    for(int i = 0; i < m_model->rowCount(); i++)
        targets.enqueue(m_model->item(i, 0)->text());

    emit reScan(targets);
    accept();
}

void FailedScansDialog::on_buttonCancel_clicked(){
    accept();
}

void FailedScansDialog::on_tableView_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

    /* check if user right clicked on items else dont show the context menu */
    if(!selectionModel->isSelected(ui->tableView->currentIndex()))
        return;

    /* creating the context menu */
    QMenu menu(this);

    /* adding actions */
    menu.addAction("Copy", this, [=](){
        QClipboard *clipboard = QGuiApplication::clipboard();

        QString clipboardData;
        foreach(const QModelIndex &index, selectionModel->selectedIndexes()){
            if(index.column() == 0)
                clipboardData.append(index.data().toString().append("\n"));
        }

        clipboard->setText(clipboardData.trimmed());
    });

    menu.addAction("Remove", this, [=](){
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            m_model->removeRow(index.row());
    });

    /* showing the context menu */
    menu.exec(QCursor::pos());
}

void FailedScansDialog::on_buttonCopy_clicked(){
    QClipboard *clipboard = QGuiApplication::clipboard();

    QString clipboardData;
    for(int i = 0; i != m_model->rowCount(); ++i)
        clipboardData.append(m_model->index(i,0).data().toString()).append("\n");

    clipboard->setText(clipboardData.trimmed());
}
