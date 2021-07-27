#include "Project.h"
#include "ui_Project.h"


Project::Project(QWidget *parent, ResultsModel *resultsModel) :QWidget(parent), ui(new Ui::Project),
    m_resultsModel(resultsModel),
    m_proxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);
    //...
    m_proxyModel->setSourceModel(m_resultsModel->project->model);
    m_proxyModel->setRecursiveFilteringEnabled(true);
    m_proxyModel->setFilterKeyColumn(0);
    //...
    ui->treeView->setModel(m_resultsModel->project->model);
    ui->treeView->expandAll();
    //...
    ui->lineEdit_inScope->setPlaceholderText("e.g google");
    ui->lineEdit_outScope->setPlaceholderText("e.g google");
}
Project::~Project(){
    delete m_proxyModel;
    delete ui;
}

void Project::updateFilter(){
    ///
    /// adding the targets to the proxy model filter....
    ///
    QString pattern("(subdomains|records|tld|a|aaaa|ns|mx|cname|txt|srv");
    for(int i = 0; i < ui->listWidget_inScope->count(); i++){
        pattern.append("|"+ui->listWidget_inScope->item(i)->text());
    }
    pattern.append(")");
    QRegExp regExp(pattern, Qt::CaseInsensitive);
    m_proxyModel->setFilterRegExp(regExp);
    ui->treeView->setModel(m_proxyModel);
}

/*********************************** IN-SCOPE **********************************/
void Project::on_pushButton_clearInScope_clicked(){
    ui->listWidget_inScope->clear();
    updateFilter();
}
void Project::on_pushButton_removeInScope_clicked(){
    int selectionCount = ui->listWidget_inScope->selectedItems().count();
    if(selectionCount){
        qDeleteAll(ui->listWidget_inScope->selectedItems());
    }
    updateFilter();
}
void Project::on_pushButton_loadInScope_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_inScope->addItem(in.readLine());
        }
        file.close();
    }else{
        QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
    }
    updateFilter();
}
void Project::on_pushButton_addInScope_clicked(){
    if(ui->lineEdit_inScope->text() == EMPTY){
        return;
    }
    ui->listWidget_inScope->addItem(ui->lineEdit_inScope->text());
    ui->lineEdit_inScope->clear();
    updateFilter();
}
void Project::on_lineEdit_inScope_returnPressed(){
    on_pushButton_addInScope_clicked();
}

/************************************ OUT-SCOPE *********************************/
void Project::on_pushButton_clearOutScope_clicked(){
    ui->listWidget_outScope->clear();
}
void Project::on_pushButton_removeOutScope_clicked(){
    int selectionCount = ui->listWidget_outScope->selectedItems().count();
    if(selectionCount){
        qDeleteAll(ui->listWidget_outScope->selectedItems());
    }
}
void Project::on_pushButton_loadOutScope_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->listWidget_outScope->addItem(in.readLine());
        }
        file.close();
    }else{
        QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
    }
}
void Project::on_pushButton_addOutScope_clicked(){
    if(ui->lineEdit_outScope->text() != EMPTY){
        ui->listWidget_outScope->addItem(ui->lineEdit_outScope->text());
        ui->lineEdit_outScope->clear();
    }
}
void Project::on_lineEdit_outScope_returnPressed(){
    on_pushButton_addOutScope_clicked();
}
