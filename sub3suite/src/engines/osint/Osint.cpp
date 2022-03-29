/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief : OSINT Engine, for obtaining different osint info from different input targets.
*/

#include "Osint.h"
#include "ui_Osint.h"

#include <QDateTime>
#include <QClipboard>

#include "src/utils/utils.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/PassiveConfigDialog.h"


Osint::Osint(QWidget *parent, ProjectModel *project): AbstractEngine(parent, project),
    ui(new Ui::Osint),
    m_scanConfig(new ScanConfig),
    m_scanArgs(new ScanArgs),
    m_targetListModel_host(new QStringListModel),
    m_targetListModel_ip(new QStringListModel),
    m_targetListModel_asn(new QStringListModel),
    m_targetListModel_cidr(new QStringListModel),
    m_targetListModel_ssl(new QStringListModel),
    m_targetListModel_email(new QStringListModel),
    m_targetListModel_query(new QStringListModel),
    m_model_subdomainIp(new QStandardItemModel),
    m_model_subdomain(new QStandardItemModel),
    m_model_ip(new QStandardItemModel),
    m_model_email(new QStandardItemModel),
    m_model_url(new QStandardItemModel),
    m_model_asn(new QStandardItemModel),
    m_model_ssl(new QStandardItemModel),
    m_model_cidr(new QStandardItemModel)
{
    this->initUI();
    this->initModules();

    /* ... */
    m_scanArgs->config = m_scanConfig;

    /* target models */
    ui->targets->setListName(tr("Targets"));
    ui->targets->setListModel(m_targetListModel_host);

    /* results models */
    m_model_subdomainIp->setHorizontalHeaderLabels({tr(" Subdomains"), tr(" IpAddresses")});
    m_model_subdomain->setHorizontalHeaderLabels({tr(" Subdomains")});
    m_model_ip->setHorizontalHeaderLabels({tr(" IpAddresses")});
    m_model_email->setHorizontalHeaderLabels({tr(" Emails")});
    m_model_url->setHorizontalHeaderLabels({tr(" URL")});
    m_model_asn->setHorizontalHeaderLabels({tr(" ASN"), tr(" Name")});
    m_model_ssl->setHorizontalHeaderLabels({tr(" SSL")});
    m_model_cidr->setHorizontalHeaderLabels({tr(" CIDR")});

    /* proxy model */
    proxyModel->setSourceModel(m_model_subdomain);
    proxyModel->setFilterKeyColumn(0);
    ui->tableViewResults->setModel(proxyModel);
}
Osint::~Osint(){
    delete m_model_cidr;
    delete m_model_ssl;
    delete m_model_asn;
    delete m_model_url;
    delete m_model_email;
    delete m_model_ip;
    delete m_model_subdomain;
    delete m_model_subdomainIp;
    delete m_targetListModel_query;
    delete m_targetListModel_host;
    delete m_targetListModel_email;
    delete m_targetListModel_cidr;
    delete m_targetListModel_ssl;
    delete m_targetListModel_asn;
    delete m_targetListModel_ip;
    delete ui;
}

void Osint::initUI(){
    ui->setupUi(this);

    /* setting properties */
    ui->labelIn->setProperty("s3s_color", true);
    ui->labelOut->setProperty("s3s_color", true);
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);

    ui->page_keyrequired->setProperty("has_border", true);
    ui->page_keynotrequired->setProperty("has_border", true);

    /* setting placeholdertxt */
    ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_DOMAIN));
    ui->lineEditFilter->setPlaceholderText(tr("Filter..."));

    /* disabling and hiding widgets */
    ui->buttonStop->setDisabled(true);
    ui->progressBar->hide();
    ui->targets->hide();
    ui->comboBoxFilter->hide();

    /* ... */
    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}

void Osint::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Osint::on_buttonStart_clicked(){
    if(ui->checkBoxMultipleTargets->isChecked() && ui->targets->getlistModel()->rowCount() == 0){
        QMessageBox::warning(this, tr("Error!"), tr("Please Enter Target For Enumerations!"));
        return;
    }
    if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, tr("Error!"), tr("Please Enter Targets For Enumerations!"));
        return;
    }

    status->isRunning = true;
    status->isNotActive = false;
    status->isStopped = false;
    status->isPaused = false;

    this->startScan();
}

void Osint::on_buttonStop_clicked(){
    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void Osint::on_buttonKeys_clicked(){
    ApiKeysDialog *apiKeysDialog = new ApiKeysDialog(this);
    apiKeysDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeysDialog->show();
}

void Osint::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this, m_scanConfig);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->loadConfig_osint();
    scanConfig->show();
}

void Osint::on_lineEditFilter_textChanged(const QString &filterKeyword){
    switch(ui->comboBoxOutput->currentIndex()){
    case osint::OUTPUT::SUBDOMAINIP:
        proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
        break;
    case osint::OUTPUT::ASN:
        proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
        break;
    case osint::OUTPUT::SUBDOMAIN:
        proxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::IP:
        proxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::EMAIL:
        proxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::URL:
        proxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::CERT:
        proxyModel->setFilterKeyColumn(0);
        break;
    case osint::OUTPUT::CIDR:
        proxyModel->setFilterKeyColumn(0);
        break;
    }

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->tableViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Osint::on_comboBoxInput_currentIndexChanged(int index){
    /* clearing the target first, since another type of input is introduced */
    ui->lineEditTarget->clear();

    /* setting a respective placeholdertext on the target line edit */
    switch (index) {
    case INPUT::HOSTNAME:
        ui->targets->setListModel(m_targetListModel_host);
        ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_DOMAIN));
        break;
    case INPUT::IP:
        ui->targets->setListModel(m_targetListModel_ip);
        ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_IP));
        break;
    case INPUT::EMAIL:
        ui->targets->setListModel(m_targetListModel_email);
        ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_EMAIL));
        break;
    case INPUT::URL:
        ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_URL));
        break;
    case INPUT::ASN:
        ui->targets->setListModel(m_targetListModel_asn);
        ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_ASN));
        break;
    case INPUT::CERT:
        ui->targets->setListModel(m_targetListModel_ssl);
        ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_SSLCERT));
        break;
    case INPUT::CIDR:
        ui->targets->setListModel(m_targetListModel_cidr);
        ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_CIDR));
        break;
    case INPUT::QUERY_TERM:
        ui->targets->setListModel(m_targetListModel_query);
        ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_QUERY));
    }

    /* only show modules that supports both input-type and output-type */
    this->initModules();
}

void Osint::on_comboBoxOutput_currentIndexChanged(int index){
    switch(index){
    case osint::OUTPUT::SUBDOMAINIP:
        proxyModel->setSourceModel(m_model_subdomainIp);
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->addItems({tr("Subdomain"), tr("IP")});
        ui->comboBoxFilter->show();
        ui->labelResultsInfo->setText(tr("Enumerated Subdomains"));
        break;
    case osint::OUTPUT::SUBDOMAIN:
        proxyModel->setSourceModel(m_model_subdomain);
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText(tr("Enumerated Subdomains"));
        break;
    case osint::OUTPUT::IP:
        proxyModel->setSourceModel(m_model_ip);
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText(tr("Enumerated Ip"));
        break;
    case osint::OUTPUT::EMAIL:
        proxyModel->setSourceModel(m_model_email);
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText(tr("Enumerated Emails"));
        break;
    case osint::OUTPUT::URL:
        proxyModel->setSourceModel(m_model_url);
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText(tr("Enumerated Urls"));
        break;
    case osint::OUTPUT::ASN:
        proxyModel->setSourceModel(m_model_asn);
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->addItems({tr("ASN"), tr("Name")});
        ui->comboBoxFilter->show();
        ui->labelResultsInfo->setText(tr("Enumerated ASNs"));
        break;
    case osint::OUTPUT::CERT:
        proxyModel->setSourceModel(m_model_ssl);
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText(tr("Enumerated Certificates"));
        break;
    case osint::OUTPUT::CIDR:
        proxyModel->setSourceModel(m_model_cidr);
        ui->comboBoxFilter->clear();
        ui->comboBoxFilter->hide();
        ui->labelResultsInfo->setText(tr("Enumerated Cidr"));
        break;
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());

    /* only show modules that supports both input-type and output-type */
    this->initModules();
}

void Osint::on_checkBoxMultipleTargets_stateChanged(int state){
    if(state == Qt::Checked)
        ui->targets->show();
    else
        ui->targets->hide();
}

void Osint::log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Osint::initConfigValues(){
    CONFIG.beginGroup(CFG_OSINT);
    m_scanConfig->timeout = CONFIG.value(CFG_VAL_TIMEOUT).toInt();
    m_scanConfig->maxPage = CONFIG.value(CFG_VAL_MAXPAGES).toInt();
    m_scanConfig->autosaveToProject = CONFIG.value(CFG_VAL_AUTOSAVE).toBool();
    m_scanConfig->noDuplicates = CONFIG.value(CFG_VAL_DUPLICATES).toBool();
    m_scanArgs->config->setTimeout = CONFIG.value(CFG_VAL_SETTIMEOUT).toBool();
    CONFIG.endGroup();
}
