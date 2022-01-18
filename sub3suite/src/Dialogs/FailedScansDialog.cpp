/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "FailedScansDialog.h"
#include "ui_FailedScansDialog.h"

#include <QMenu>
#include <QAction>


FailedScansDialog::FailedScansDialog(QWidget *parent, QMap<QString,QString> targets) :
    QDialog(parent),
    ui(new Ui::FailedScansDialog),
    m_model(new QStandardItemModel)
{
    ui->setupUi(this);

    // initializing the model
    m_model->setColumnCount(2);
    m_model->setHorizontalHeaderLabels({"Target", "Error"});

    // appending the targets and their errors on the model
    QStringList targetList = targets.keys();
    foreach(const QString &target, targetList)
        m_model->appendRow({new QStandardItem(target), new QStandardItem(targets.value(target))});

    // model to the tableview
    ui->tableView->setModel(m_model);
}
FailedScansDialog::~FailedScansDialog(){
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

    /* check if user right clicked on items else dont show the context menu */
    if(!ui->tableView->selectionModel()->isSelected(ui->tableView->currentIndex()))
        return;

    /* getting the selected items */
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();

    /* action */
    QAction remove("Remove");
    connect(&remove, &QAction::triggered, this, [=](){
        foreach(const QModelIndex &index, selectionModel->selectedIndexes())
            m_model->removeRow(index.row());
    });

    /* creating the context menu */
    QMenu menu(this);

    /* adding to mainMenu */
    menu.addAction(&remove);

    /* showing the context menu */
    menu.exec(QCursor::pos());
}
