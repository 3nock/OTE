/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Ssl.h"
#include "ui_Ssl.h"

#include <QSslKey>
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/utils/Config.h"
#include "src/utils/Definitions.h"
#include "src/models/SSLModel.h"
#include "src/modules/active/SSLScanner.h"


Ssl::Ssl(QWidget *parent, ProjectModel *project): AbstractEngine(parent, project), ui(new Ui::Ssl),
    m_scanConfig(new ssl::ScanConfig),
    m_scanArgs(new ssl::ScanArgs),
    m_scanStats(new ssl::ScanStat),
    m_targetListModel(new QStringListModel),
    m_resultModelSubdomain(new QStandardItemModel),
    m_resultModelCertId(new QStandardItemModel),
    m_resultModelCertInfo(new QStandardItemModel),
    m_resultProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    /* init */
    ui->targets->setListName("Targets");
    ui->targets->setListModel(m_targetListModel);

    /* result models */
    m_resultModelSubdomain->setHorizontalHeaderLabels({"Subdomains"});
    m_resultModelCertId->setHorizontalHeaderLabels({"Certificate Hash"});
    m_resultModelCertInfo->setHorizontalHeaderLabels({"Property", "Value"});
    m_resultProxyModel->setSourceModel(m_resultModelSubdomain);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->treeViewResults->setModel(m_resultProxyModel);

    /* enabling and disabling widgets */
    ui->progressBar->hide();
    ui->comboBoxOption->hide();
    ui->buttonStop->setDisabled(true);

    /* placeholdertxt */
    ui->lineEditFilter->setPlaceholderText("filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));

    /* initiate all actions for the context menus */
    this->m_initActions();

    /* ... */
    m_scanArgs->config = m_scanConfig;

    /* ... */
    this->m_getConfigValues();
}
Ssl::~Ssl(){
    delete m_resultProxyModel;
    delete m_resultModelCertId;
    delete m_resultModelCertInfo;
    delete m_resultModelSubdomain;
    delete m_targetListModel;
    delete m_scanStats;
    delete m_scanArgs;
    delete m_scanConfig;
    delete ui;
}

void Ssl::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void Ssl::on_buttonStart_clicked(){
    ///
    /// Start scan...
    ///
    if(status->isNotActive){
        if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
            QMessageBox::warning(this, "Error!", "Please Enter the Target for Enumeration!");
            return;
        }
        if(ui->checkBoxMultipleTargets->isChecked() && m_targetListModel->rowCount() < 1){
            QMessageBox::warning(this, "Error!", "Please Enter the Targets for Enumeration!");
            return;
        }

        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText("Pause");

        status->isRunning = true;
        status->isNotActive = false;
        status->isStopped = false;
        status->isPaused = false;

        /* start scan */
        this->m_startScan();

        /* logs */
        m_log("------------------ start ----------------");
        qInfo() << "Scan Started";
        return;
    }
    ///
    /// Pause scan...
    ///
    if(status->isRunning){
        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText("Resume");

        status->isPaused = true;
        status->isRunning = false;
        status->isStopped = false;
        status->isNotActive = false;

        /* pause scan */
        emit pauseScanThread();

        /* logs */
        m_log("------------------ Paused ----------------");
        qInfo() << "Scan Paused";
        return;
    }
    ///
    /// Resume scan...
    ///
    if(status->isPaused){
        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText("Pause");

        status->isRunning = true;
        status->isPaused = false;
        status->isStopped = false;
        status->isNotActive = false;

        /* resume scan */
        emit resumeScanThread();

        /* logs */
        m_log("------------------ Resumed ----------------");
        qInfo() << "Scan Resumed";
    }
}

void Ssl::on_buttonStop_clicked(){
    if(status->isPaused)
        emit resumeScanThread();

    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void Ssl::on_comboBoxOutput_currentIndexChanged(int index){
    switch (index) {
    case 0: // subdomain
        m_resultProxyModel->setSourceModel(m_resultModelSubdomain);
        ui->comboBoxOption->hide();
        ui->labelInfo->setText("Enumerated Subdomains:");
        ui->treeViewResults->setIndentation(0);
        break;
    case 1: // cert id
        m_resultProxyModel->setSourceModel(m_resultModelCertId);
        ui->comboBoxOption->show();
        ui->labelInfo->setText("Enumerated Certificates:");
        ui->treeViewResults->setIndentation(0);
        break;
    case 2: // raw cert
        m_resultProxyModel->setSourceModel(m_resultModelCertInfo);
        ui->comboBoxOption->hide();
        ui->labelInfo->setText("Enumerated Certificates:");
        ui->treeViewResults->setIndentation(20);
        break;
    }

    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Ssl::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Ssl::on_lineEditFilter_textChanged(const QString &filterKeyword){
    if(ui->checkBoxRegex->isChecked())
        m_resultProxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        m_resultProxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(m_resultProxyModel);
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Ssl::m_getConfigValues(){
    m_scanArgs->config->timeout = CONFIG_SSL.value("timeout").toInt();
    m_scanArgs->config->threads = CONFIG_SSL.value("threads").toInt();
    m_scanArgs->config->noDuplicates = CONFIG_SSL.value("noDuplicates").toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG_SSL.value("autosaveToProject").toBool();
}

void Ssl::m_log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}
