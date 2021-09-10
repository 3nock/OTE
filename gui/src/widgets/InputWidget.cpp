#include "InputWidget.h"
#include "ui_InputWidget.h"

InputWidget::InputWidget(QWidget *parent) : QWidget(parent), ui(new Ui::InputWidget)
{
    ui->setupUi(this);
    ///
    /// setting name...
    ///
    ui->lineEdit->setPlaceholderText("Enter new item...");
    listWidget = ui->listWidget;
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
    /// appending the word to the list...
    ///
    ui->listWidget->addItem(item);
    ui->labelCount->setNum(ui->listWidget->count());
}

void InputWidget::add(const QStringList &itemList){
    ///
    /// checks...
    ///
    if(itemList.isEmpty())
        return;
    ///
    /// appending list contents to the list widget...
    ///
    foreach(const QString &item, itemList){
        int qsetSize = m_wordlist.size();
        m_wordlist.insert(item);
        if(m_wordlist.size() == qsetSize)
            return;
        ///
        /// appending the word to the list...
        ///
        ui->listWidget->addItem(item);
    }
    ui->labelCount->setNum(ui->listWidget->count());
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
        if(m_wordlist.size() > qsetSize)
            ui->listWidget->addItem(item);
    }
    ui->labelCount->setNum(ui->listWidget->count());
    file.close();
}

void InputWidget::on_buttonClear_clicked(){
    ui->listWidget->clear();
    ui->labelCount->clear();
    m_wordlist.clear();
}

void InputWidget::on_buttonRemove_clicked(){
    ///
    /// checks...
    ///
    QList<QListWidgetItem *> selectedItems = ui->listWidget->selectedItems();
    if(selectedItems.count() <= 0)
        return;
    foreach(QListWidgetItem *item, selectedItems)
        m_wordlist.remove(item->text());
    qDeleteAll(selectedItems);
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
        if(m_wordlist.size() > qsetSize)
            ui->listWidget->addItem(item);
    }
    ui->labelCount->setNum(ui->listWidget->count());
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
