#include "InputWidget.h"
#include "ui_InputWidget.h"


InputWidget::InputWidget(QWidget *parent) : QWidget(parent), ui(new Ui::InputWidget)
{
    ui->setupUi(this);

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
    ui->labelName->setText(listName+" Count: ");
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
    /* checks */
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Error Ocurred!", "Failed To Open the File!");
        return;
    }

    /* loading the file contents */
    QString item;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        item = in.readLine();

        /* appending the item to the list */
        if(m_listModel->insertRow(m_listModel->rowCount()))
            m_listModel->setData(m_listModel->index(m_listModel->rowCount()-1, 0), item);
    }
    ui->labelCount->setNum(m_listModel->rowCount());
    file.close();
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

    /* load file contents */
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Error Ocurred!", "Failed To Open the File!");
        return;
    }

    /* loading the file contents */
    QString item;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        item = in.readLine();

        /* appending the item to the list */
        if(m_listModel->insertRow(m_listModel->rowCount()))
            m_listModel->setData(m_listModel->index(m_listModel->rowCount()-1, 0), item);
    }
    ui->labelCount->setNum(m_listModel->rowCount());
    file.close();
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
