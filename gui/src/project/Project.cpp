#include "Project.h"
#include "ui_Project.h"
//...
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

/*
 * option to remove all passives that are in active...
 *
 * produce transforms-like graphs
 * that can fetch more data/info by clicking on the node..
 *
 */
Project::Project(QWidget *parent, ProjectDataModel *projectDataModel) :QWidget(parent), ui(new Ui::Project),
    m_projectDataModel(projectDataModel),
    m_proxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);
    ///
    /// for analysis tabs...
    ///
    /*
    general = new GeneralAnalysis(this);
    specific = new SpecificAnalysis(this);
    ui->tabWidgetProject->insertTab(1, general, "General");
    ui->tabWidgetProject->insertTab(2, specific, "Specific");
    */
    ///
    /// setting up signals and slots...
    ///
    //...
    //...
    m_proxyModel->setSourceModel(m_projectDataModel->projectModel);
    m_proxyModel->setRecursiveFilteringEnabled(true);
    //...
    ui->treeView->setModel(m_proxyModel);
    ui->treeView->expandAll();
    ui->treeView->setColumnHidden(1, true);
    //...
    ui->lineEdit_inScope->setPlaceholderText("e.g google ...");
    ui->lineEdit_filter->setPlaceholderText("Enter filter...");
    //...
    ui->frame_filter->hide();
    ui->treeView->setColumnHidden(1, true);
    ui->treeView->setColumnHidden(2, true);
}
Project::~Project(){
    delete m_proxyModel;
    delete ui;
}

void Project::updateFilter(){
    ///
    /// adding the targets to the proxy model filter....
    ///
    // QString pattern("(subdomains|records|tld|a|aaaa|ns|mx|cname|txt|srv");
    QString pattern("(");
    for(int i = 0; i < ui->listWidget_inScope->count(); i++){
        if(i > 0)
            pattern.append("|");
        pattern.append(ui->listWidget_inScope->item(i)->text());
    }
    pattern.append(")");
    m_proxyModel->setFilterKeyColumn(2);
    QRegExp regExp(pattern, Qt::CaseInsensitive);
    m_proxyModel->setFilterRegExp(regExp);
    ui->treeView->setModel(m_proxyModel);
}

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
    QString filename = QFileDialog::getOpenFileName(this, "Load From File...", "./");
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
        QMessageBox::warning(this, "Error!", "Failed To Open the File!");
    }
    updateFilter();
}

void Project::on_pushButton_addInScope_clicked(){
    if(ui->lineEdit_inScope->text().isEmpty())
        return;
    ///
    /// ...
    ///
    if(ui->lineEdit_inScope->text().contains("."))
    {
        ui->listWidget_inScope->addItem(ui->lineEdit_inScope->text().split(".")[0]);
    }
    else{
        ui->listWidget_inScope->addItem(ui->lineEdit_inScope->text());
    }
    ui->lineEdit_inScope->clear();
    updateFilter();
}

void Project::on_lineEdit_inScope_returnPressed(){
    on_pushButton_addInScope_clicked();
}

void Project::on_checkBox_enableFilter_clicked(bool checked){
    if(checked){
        ui->frame_filter->show();
    }
    else{
        ui->frame_filter->hide();
    }
}

void Project::on_checkBox_columnIpAddress_clicked(bool checked){
    if(checked){
        ui->treeView->setColumnHidden(1, false);
    }
    else{
        ui->treeView->setColumnHidden(1, true);
    }
}

void Project::on_checkBox_columnScopeTarget_clicked(bool checked){
    if(checked){
        ui->treeView->setColumnHidden(2, false);
    }
    else{
        ui->treeView->setColumnHidden(2, true);
    }
}

void Project::on_pushButton_filter_clicked(){
    m_proxyModel->setFilterKeyColumn(ui->comboBox_filter->currentIndex());
    QRegExp regExp(ui->lineEdit_filter->text(), Qt::CaseInsensitive);
    m_proxyModel->setFilterRegExp(regExp);
    ui->treeView->setModel(m_proxyModel);
}
