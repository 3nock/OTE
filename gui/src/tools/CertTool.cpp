#include "CertTool.h"
#include "ui_CertTool.h"

#include "src/utils/Definitions.h"


CertTool::CertTool(QWidget *parent) : QDialog(parent), ui(new Ui::CertTool),
    m_certModel(new CertModel),
    m_proxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    /* setting the placeholdertxt */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_SSLCERT);

    /* setting the models */
    m_proxyModel->setSourceModel(m_certModel->model);
    ui->treeResults->setModel(m_proxyModel);
}
CertTool::~CertTool(){
    delete ui;
    delete m_certModel;
    delete m_proxyModel;
}

void CertTool::on_buttonAnalyze_clicked(){

}

void CertTool::on_checkBoxExpand_clicked(bool checked){
    if(checked)
        ui->treeResults->expandAll();
    else
        ui->treeResults->collapseAll();
}
