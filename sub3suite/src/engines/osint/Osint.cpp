/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Osint.h"
#include "ui_Osint.h"

#include <QDateTime>
#include <QClipboard>
#include "src/utils/Config.h"
#include "src/utils/Definitions.h"
#include "src/dialogs/PassiveConfigDialog.h"


Osint::Osint(QWidget *parent, ProjectModel *project): AbstractEngine(parent, project),
    ui(new Ui::Osint),
    m_targetListModelHostname(new QStringListModel),
    m_targetListModelIp(new QStringListModel),
    m_targetListModelAsn(new QStringListModel),
    m_targetListModelCidr(new QStringListModel),
    m_targetListModelCert(new QStringListModel),
    m_targetListModelEmail(new QStringListModel),
    m_resultModelSubdomainIp(new QStandardItemModel),
    m_resultModelSubdomain(new QStandardItemModel),
    m_resultModelIp(new QStandardItemModel),
    m_resultModelEmail(new QStandardItemModel),
    m_resultModelUrl(new QStandardItemModel),
    m_resultModelAsn(new QStandardItemModel),
    m_resultModelCert(new QStandardItemModel),
    m_resultModelCidr(new QStandardItemModel),
    m_resultProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    ui->labelIn->setProperty("s3s_color", true);
    ui->labelOut->setProperty("s3s_color", true);

    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);

    /* init... */
    ui->targets->setListName("Targets");
    ui->targets->setListModel(m_targetListModelHostname);

    /* results models */
    m_resultModelSubdomainIp->setHorizontalHeaderLabels({" Subdomains", " IpAddresses"});
    m_resultModelSubdomain->setHorizontalHeaderLabels({" Subdomains"});
    m_resultModelIp->setHorizontalHeaderLabels({" IpAddresses"});
    m_resultModelEmail->setHorizontalHeaderLabels({" Emails"});
    m_resultModelUrl->setHorizontalHeaderLabels({" Urls"});
    m_resultModelAsn->setHorizontalHeaderLabels({" Asn", " Name"});
    m_resultModelCert->setHorizontalHeaderLabels({" SSL-Cert FingerPrint"});
    m_resultModelCidr->setHorizontalHeaderLabels({" Cidr"});
    m_resultProxyModel->setSourceModel(m_resultModelSubdomain);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->tableViewResults->setModel(m_resultProxyModel);

    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    /* ... */
    m_currentPath = QApplication::applicationDirPath();
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
    ui->lineEditFilter->setPlaceholderText("Filter...");

    /* hide widgets... */
    ui->buttonStop->setDisabled(true);
    ui->progressBar->hide();
    ui->targets->hide();
    ui->comboBoxFilter->hide();

    /* modules */
    this->m_initModules();

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));

    /* initiate all actions for the context menus */
    this->m_initActions();
}
Osint::~Osint(){
    delete m_resultProxyModel;
    delete m_resultModelCidr;
    delete m_resultModelCert;
    delete m_resultModelAsn;
    delete m_resultModelUrl;
    delete m_resultModelEmail;
    delete m_resultModelIp;
    delete m_resultModelSubdomain;
    delete m_resultModelSubdomainIp;
    delete m_targetListModelHostname;
    delete m_targetListModelEmail;
    delete m_targetListModelCidr;
    delete m_targetListModelCert;
    delete m_targetListModelAsn;
    delete m_targetListModelIp;
    delete ui;
}

void Osint::m_infoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Osint::m_errorLog(QString log){
    QString fontedLog("<font color=\"red\">"+log+"</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml("\n"+logTime+fontedLog+"\n");
}

void Osint::on_buttonStart_clicked(){
    if(ui->checkBoxMultipleTargets->isChecked() && ui->targets->getlistModel()->rowCount() == 0){
        QMessageBox::warning(this, "Error!", "Please Target For Enumerations!");
        return;
    }
    if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please Target For Enumerations!");
        return;
    }

    this->m_startScan();
}

void Osint::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Osint::on_buttonStop_clicked(){
    emit stopScanThread();
}

void Osint::m_stopScan(){

}

void Osint::m_resumeScan(){

}

void Osint::onScanThreadEnded(){
    /* check if no active thread... */
    status->activeScanThreads--;
    if(status->activeScanThreads)
        return;

    /* reanabling the widgets... */
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);

    this->m_infoLog("------------------ End ----------------");
}

void Osint::m_clearResults(){
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        m_subdomainIpSet.clear();
        m_resultModelSubdomainIp->clear();
        m_resultModelSubdomainIp->setHorizontalHeaderLabels({"Subdomains", "IpAddresses"});
        break;
    case osint::OUTPUT::SUBDOMAIN:
        m_subdomainSet.clear();
        m_resultModelSubdomain->clear();
        m_resultModelSubdomain->setHorizontalHeaderLabels({"Subdomains"});
        break;
    case osint::OUTPUT::IP:
        m_ipSet.clear();
        m_resultModelIp->clear();
        m_resultModelIp->setHorizontalHeaderLabels({"IpAddresses"});
        break;
    case osint::OUTPUT::EMAIL:
        m_emailSet.clear();
        m_resultModelEmail->clear();
        m_resultModelEmail->setHorizontalHeaderLabels({"Emails"});
        break;
    case osint::OUTPUT::URL:
        m_urlSet.clear();
        m_resultModelUrl->clear();
        m_resultModelUrl->setHorizontalHeaderLabels({"Urls"});
        break;
    case osint::OUTPUT::ASN:
        m_asnSet.clear();
        m_resultModelAsn->clear();
        m_resultModelAsn->setHorizontalHeaderLabels({"Asn", "Name"});
        break;
    case osint::OUTPUT::CERT:
        m_sslCertSet.clear();
        m_resultModelCert->clear();
        m_resultModelCert->setHorizontalHeaderLabels({"Asn", "Name"});
        break;
    case osint::OUTPUT::CIDR:
        m_cidrSet.clear();
        m_resultModelCert->clear();
        m_resultModelCert->setHorizontalHeaderLabels({"Cidr"});
        break;
    }
    ui->labelResultsCount->clear();

    /* clear the progressbar... */
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}


void Osint::on_buttonKeys_clicked(){
    ApiKeysDialog *apiKeysDialog = new ApiKeysDialog(this);
    apiKeysDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeysDialog->show();
}

void Osint::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}

void Osint::on_lineEditFilter_textChanged(const QString &filterKeyword){
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        m_resultProxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
        break;
    case osint::OUTPUT::SUBDOMAIN:
        m_resultProxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::IP:
        m_resultProxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::EMAIL:
        m_resultProxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::URL:
        m_resultProxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::ASN:
        m_resultProxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
        break;
    case osint::OUTPUT::CERT:
        m_resultProxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::CIDR:
        m_resultProxyModel->setFilterKeyColumn(0);
        break;
    }

    if(ui->checkBoxRegex->isChecked())
        m_resultProxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        m_resultProxyModel->setFilterFixedString(filterKeyword);

    ui->tableViewResults->setModel(m_resultProxyModel);
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Osint::on_comboBoxInput_currentIndexChanged(int index){
    /* clearing the target first, since another type of input is introduced */
    ui->lineEditTarget->clear();

    /* setting a respective placeholdertext on the target line edit */
    switch (index) {
    case INPUT::HOSTNAME:
        ui->targets->setListModel(m_targetListModelHostname);
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
        break;
    case INPUT::IP:
        ui->targets->setListModel(m_targetListModelIp);
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_IP);
        break;
    case INPUT::EMAIL:
        ui->targets->setListModel(m_targetListModelEmail);
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_EMAIL);
        break;
    case INPUT::URL:
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_URL);
        break;
    case INPUT::ASN:
        ui->targets->setListModel(m_targetListModelAsn);
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_ASN);
        break;
    case INPUT::CERT:
        ui->targets->setListModel(m_targetListModelCert);
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_SSLCERT);
        break;
    case INPUT::CIDR:
        ui->targets->setListModel(m_targetListModelCidr);
        ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_CIDR);
        break;
    }
    /* only show modules that supports both input-type and output-type */
    this->m_initModules();
}

void Osint::on_comboBoxOutput_currentIndexChanged(int index){
    switch(index){
    case osint::OUTPUT::SUBDOMAINIP:
        m_resultProxyModel->setSourceModel(m_resultModelSubdomainIp);
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->addItems({"Subdomain", "Ip"});
        ui->comboBoxFilter->show();
        ui->labelResultsInfo->setText("Enumerated Subdomains");
        break;
    case osint::OUTPUT::SUBDOMAIN:
        m_resultProxyModel->setSourceModel(m_resultModelSubdomain);
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Subdomains");
        break;
    case osint::OUTPUT::IP:
        m_resultProxyModel->setSourceModel(m_resultModelIp);
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Ip");
        break;
    case osint::OUTPUT::EMAIL:
        m_resultProxyModel->setSourceModel(m_resultModelEmail);
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Emails");
        break;
    case osint::OUTPUT::URL:
        m_resultProxyModel->setSourceModel(m_resultModelUrl);
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Urls");
        break;
    case osint::OUTPUT::ASN:
        m_resultProxyModel->setSourceModel(m_resultModelAsn);
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->addItems({"ASN", "Name"});
        ui->comboBoxFilter->show();
        ui->labelResultsInfo->setText("Enumerated ASNs");
        break;
    case osint::OUTPUT::CERT:
        m_resultProxyModel->setSourceModel(m_resultModelCert);
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Certificates");
        break;
    case osint::OUTPUT::CIDR:
        m_resultProxyModel->setSourceModel(m_resultModelCidr);
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText("Enumerated Cidr");
        break;
    }

    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
    /* only show modules that supports both input-type and output-type */
    this->m_initModules();
}

void Osint::on_checkBoxMultipleTargets_stateChanged(int state){
    if(state == Qt::Checked)
        ui->targets->show();
    else
        ui->targets->hide();
}
