#include "InputWidget.h"
#include "ui_InputWidget.h"


InputWidget::InputWidget(QWidget *parent) : QWidget(parent), ui(new Ui::InputWidget),
    listModel(new QStringListModel)
{
    ui->setupUi(this);
    ///
    /// setting name...
    ///
    ui->lineEdit->setPlaceholderText("Enter new item...");
    ui->listView->setModel(listModel);
}
InputWidget::~InputWidget(){
    delete ui;
}

void InputWidget::init(const QString &name){
    ui->labelName->setText(name+" Count: ");
}

void InputWidget::add(const QString &item){
    ///
    /// checks...
    ///
    if(item.isEmpty())
        return;
    int qsetSize = m_wordlist.size();
    m_wordlist.insert(item);
    if(m_wordlist.size() == qsetSize)
        return;
    ///
    /// appending the item to the list...
    ///
    if(listModel->insertRow(listModel->rowCount()))
        listModel->setData(listModel->index(listModel->rowCount()-1, 0), item);

    ui->labelCount->setNum(listModel->rowCount());
}

void InputWidget::add(QFile &file){
    ///
    /// checks...
    ///
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, "Error Ocurred!", "Failed To Open the File!");
        return;
    }
    ///
    /// loading the file contents...
    ///
    QString item;
    QTextStream in(&file);
    while (!in.atEnd())
    {
        item = in.readLine();
        int qsetSize = m_wordlist.size();
        m_wordlist.insert(item);
        if(m_wordlist.size() > qsetSize){
            ///
            /// appending the item to the list...
            ///
            if(listModel->insertRow(listModel->rowCount()))
                listModel->setData(listModel->index(listModel->rowCount()-1, 0), item);
        }
    }
    ui->labelCount->setNum(listModel->rowCount());
    file.close();
}

void InputWidget::on_buttonClear_clicked(){
    listModel->removeRows(0, listModel->rowCount());
    ui->labelCount->clear();
    m_wordlist.clear();
}

void InputWidget::on_buttonRemove_clicked(){
    QModelIndexList selectedIndexes(ui->listView->selectionModel()->selectedIndexes());

    for (QModelIndexList::const_iterator i = selectedIndexes.constEnd() - 1;
            i >= selectedIndexes.constBegin(); --i) {
        listModel->removeRow(i->row());
    }
    ui->labelCount->setNum(listModel->rowCount());
}

void InputWidget::on_buttonLoad_clicked(){
    ///
    /// check...
    ///
    QString filename = QFileDialog::getOpenFileName(this, "Load From File...", "./");
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    this->add(file);
}

void InputWidget::on_buttonAdd_clicked(){
    this->add(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void InputWidget::on_lineEdit_returnPressed(){
    this->add(ui->lineEdit->text());
    ui->lineEdit->clear();
}
