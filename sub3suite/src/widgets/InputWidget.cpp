/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "InputWidget.h"
#include "ui_InputWidget.h"

#include <QClipboard>


InputWidget::InputWidget(QWidget *parent) : QWidget(parent),
    ui(new Ui::InputWidget)
{
    ui->setupUi(this);

    ui->labelCount->setProperty("dark", true);

    ui->lineEdit->setPlaceholderText("Enter new item...");
}
InputWidget::~InputWidget(){
    delete ui;
}

QStringListModel *InputWidget::getlistModel(){
    return m_listModel;
}

void InputWidget::setListModel(QStringListModel *listModel){
    ui->listView->setModel(listModel);
    m_listModel = listModel;
}

void InputWidget::setListName(const QString& listName){
    ui->labelName->setText(listName);
}

void InputWidget::add(const QString& item){
    /* checks */
    if(item.isEmpty())
        return;

    /* appending the item to the list */
    if(m_listModel->insertRow(m_listModel->rowCount()))
        m_listModel->setData(m_listModel->index(m_listModel->rowCount()-1, 0), item);

    ui->labelCount->setNum(m_listModel->rowCount());
}

void InputWidget::add(QFile& file){
    /* get the stringList from model */
    QStringList list(m_listModel->stringList());

    /* loading wordlist from file to wordlist stringlistmodel */
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd())
            list.push_back(in.readLine());

        file.close();
    }
    else
        QMessageBox::warning(this, "Error Ocurred!", "Failed To Open the File!");

    /* set the stringList to the model */
    m_listModel->setStringList(list);

    /* update count status */
    ui->labelCount->setNum(m_listModel->rowCount());
}

void InputWidget::updateSize(){
    ui->labelCount->setNum(m_listModel->rowCount());
}

void InputWidget::on_buttonLoad_clicked(){
    /* get file to load wordlist from */
    QString filename = QFileDialog::getOpenFileName(this, "Load From File...", "./");
    if(filename.isEmpty()){
        return;
    }

    /* get the stringList from model */
    QStringList list(m_listModel->stringList());

    /* loading wordlist from file to wordlist stringlistmodel */
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd())
            list.push_back(in.readLine());

        file.close();
    }
    else
        QMessageBox::warning(this, "Error Ocurred!", "Failed To Open the File!");

    /* set the stringList to the model */
    m_listModel->setStringList(list);

    /* update count status */
    ui->labelCount->setNum(m_listModel->rowCount());
}

void InputWidget::on_buttonAdd_clicked(){
    this->add(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void InputWidget::on_lineEdit_returnPressed(){
    this->add(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void InputWidget::on_buttonClear_clicked(){
    m_listModel->removeRows(0, m_listModel->rowCount());
    ui->labelCount->clear();
}

void InputWidget::on_buttonRemove_clicked(){
    QModelIndexList selectedIndexes(ui->listView->selectionModel()->selectedIndexes());

    for(QModelIndexList::const_iterator i = selectedIndexes.constEnd()-1; i >= selectedIndexes.constBegin(); --i)
        m_listModel->removeRow(i->row());

    ui->labelCount->setNum(m_listModel->rowCount());
}

void InputWidget::on_buttonPaste_clicked(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QStringList wordlist = clipboard->text().split("\n");

    /* get the stringList from model */
    QStringList list(m_listModel->stringList());

    /* append clipboard data */
    list.append(wordlist);

    /* set the stringList to the model */
    m_listModel->setStringList(list);

    /* update count status */
    ui->labelCount->setNum(m_listModel->rowCount());
}
