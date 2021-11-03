#include "IpChecker.h"
#include "ui_IpChecker.h"

IpChecker::IpChecker(QWidget *parent) : QDialog(parent), ui(new Ui::IpChecker),
    ipModel(new IpModel)
{
    ui->setupUi(this);
    ///
    /// placeholder texts...
    ///
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText("Target eg 1.1.1.1");
    ///
    /// setting model with tableView...
    ///
    ui->treeResults->setModel(ipModel->model);
}
IpChecker::~IpChecker(){
    delete ui;
}

void IpChecker::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}
