//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "MultipleTargetsDialog.h"
#include "ui_MultipleTargetsDialog.h"

#include <QClipboard>

#include "src/core/Config.h"

MultipleTargetsDialog::MultipleTargetsDialog(QStringList *targets, const QString &description, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MultipleTargetsDialog),
    mTargets(targets),
    mModel(new QStringListModel)
{
    ui->setupUi(this);

    ui->lineEdit->setPlaceholderText(tr("Enter new target"));

    ui->label->setText(description);

    ui->listView->setModel(mModel);
}

MultipleTargetsDialog::~MultipleTargetsDialog(){
    delete mModel;
    delete ui;
}

void MultipleTargetsDialog::on_buttonLoad_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, tr("Load From File..."), OTE::gConfig.general.recentDirectory);
    if(filename.isEmpty()){
        return;
    }

    QStringList list(mModel->stringList());

    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
            list.push_back(in.readLine());

        file.close();
    }
    else
        QMessageBox::warning(this, tr("Error!"), tr("Failed To Open the File!"));

    mModel->setStringList(list);
}

void MultipleTargetsDialog::on_buttonAdd_clicked(){
    QString item = ui->lineEdit->text();
    if(item.isEmpty())
        return;

    if(mModel->insertRow(mModel->rowCount()))
        mModel->setData(mModel->index(mModel->rowCount()-1, 0), item);

    ui->lineEdit->clear();
}

void MultipleTargetsDialog::on_buttonClear_clicked(){
    mModel->removeRows(0, mModel->rowCount());
    ui->lineEdit->clear();
}

void MultipleTargetsDialog::on_buttonRemove_clicked(){
    QModelIndexList selectedIndexes(ui->listView->selectionModel()->selectedIndexes());

    for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
        mModel->removeRow(i->row());
}

void MultipleTargetsDialog::on_buttonPaste_clicked(){
    QStringList list = mModel->stringList();
    list << QGuiApplication::clipboard()->text().split("\n");
    mModel->setStringList(list);
}


void MultipleTargetsDialog::on_buttonStart_clicked()
{
    QStringList targets = mModel->stringList();

    if(targets.isEmpty())
    {
        QMessageBox::warning(this, tr("Error"), tr("Please Enter Targets"));
        return;
    }

    mTargets->append(targets);
    accept();
}
