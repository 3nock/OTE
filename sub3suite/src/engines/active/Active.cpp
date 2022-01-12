/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Active.h"
#include "ui_Active.h"

#include <QThread>
#include <QDateTime>
#include "src/utils/Config.h"
#include "src/utils/Definitions.h"
#include "src/dialogs/ActiveConfigDialog.h"


Active::Active(QWidget *parent, ProjectDataModel *project) : AbstractEngine(parent, project), ui(new Ui::Active),
    m_scanConfig(new active::ScanConfig),
    m_scanArgs(new active::ScanArgs),
    m_scanStats(new active::ScanStat),
    m_targetListModel(new QStringListModel),
    m_resultModel(new QStandardItemModel),
    m_resultProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    /* targets */
    ui->targets->setListName("Targets");
    ui->targets->setListModel(m_targetListModel);

    /* result model */
    m_resultModel->setHorizontalHeaderLabels({"Host", "IpAddress"});
    m_resultProxyModel->setSourceModel(m_resultModel);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->tableViewResults->setModel(m_resultProxyModel);

    /* hiding widgets */
    ui->progressBar->hide();
    ui->buttonStop->setDisabled(true);

    /* equsubdomainIpy seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));

    /* placeholdertext */
    ui->lineEditFilter->setPlaceholderText("filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);

    /* ... */
    this->m_initActions();

    /* config... */
    m_scanArgs->config = m_scanConfig;

    /* get prev config values */
    this->m_getConfigValues();
}
Active::~Active(){
    delete m_scanConfig;
    delete m_scanArgs;
    delete m_scanStats;
    delete m_targetListModel;
    delete m_resultModel;
    delete m_resultProxyModel;
    delete ui;
}

void Active::on_buttonStart_clicked(){
    /* checking if subdomainIp requirements are satisfied before scan if not prompt error
       then exit function... */
    if(!(m_targetListModel->rowCount() > 0)){
        QMessageBox::warning(this, "Error!", "Please Enter the subdomains Wordlist for Enumeration!");
        return;
    }

    /* disabling and Enabling widgets... */
    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();

    /* Resetting the scan arguments values... */
    m_scanArgs->progress = 0;
    ui->progressBar->reset();

    /* Getting scan arguments....
    if(ui->comboBoxOption->currentIndex() == ACTIVE::DNS){
        m_scanArgs->checkActiveService = false;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::HTTP){
        m_scanArgs->checkActiveService = true;
        m_scanArgs->service = 80;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::HTTPS){
        m_scanArgs->checkActiveService = true;
        m_scanArgs->service = 443;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::FTP){
        m_scanArgs->checkActiveService = true;
        m_scanArgs->service = 21;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::SMTP){
        m_scanArgs->checkActiveService = true;
        m_scanArgs->service = 587;
    }
    ui->progressBar->setMaximum(m_targetListModel->rowCount());

     start active subdomain enumeration... */
    this->m_startScan();
    sendStatus("[*] Testing For Active Subdomains...");
}

void Active::on_buttonStop_clicked(){
    emit stopScanThread();
    status->isStopped = true;
}

void Active::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Active::m_getConfigValues(){
    m_scanArgs->config->timeout = CONFIG_ACTIVE.value("timeout").toInt();
    m_scanArgs->config->threads = CONFIG_ACTIVE.value("threads").toInt();
    m_scanArgs->config->checkWildcard = CONFIG_ACTIVE.value("wildcard").toBool();
    m_scanArgs->config->noDuplicates = CONFIG_ACTIVE.value("noDuplicates").toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG_ACTIVE.value("autosaveToProject").toBool();

    QString record = CONFIG_ACTIVE.value("record").toString();
    if(record == "A")
        m_scanArgs->config->recordType = QDnsLookup::A;
    if(record == "AAAA")
        m_scanArgs->config->recordType = QDnsLookup::AAAA;

    int size = CONFIG_ACTIVE.beginReadArray("Nameservers");
    for (int i = 0; i < size; ++i) {
        CONFIG_ACTIVE.setArrayIndex(i);
        m_scanArgs->config->nameservers.append(CONFIG_ACTIVE.value("value").toString());
    }
    CONFIG_ACTIVE.endArray();
}
