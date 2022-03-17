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
#include "src/utils/utils.h"
#include "src/modules/active/SSLScanner.h"


Ssl::Ssl(QWidget *parent, ProjectModel *project): AbstractEngine(parent, project), ui(new Ui::Ssl),
    m_scanConfig(new ssl::ScanConfig),
    m_scanArgs(new ssl::ScanArgs),
    m_scanStats(new ssl::ScanStat),
    m_targetListModel(new QStringListModel),
    m_model_subdomain(new QStandardItemModel),
    m_model_hash(new QStandardItemModel),
    m_model_ssl(new QStandardItemModel)
{
    this->initUI();

    /* list models */
    ui->targets->setListName("Targets");
    ui->targets->setListModel(m_targetListModel);

    /* result models */
    ui->treeViewResults->setHeaderHidden(false);
    m_model_subdomain->setHorizontalHeaderLabels({tr(" Alternative Names")});
    m_model_hash->setHorizontalHeaderLabels({tr(" Certificate Hash")});
    m_model_ssl->setHorizontalHeaderLabels({tr(" SSL"), tr(" Values")});
    proxyModel->setSourceModel(m_model_subdomain);
    ui->treeViewResults->setModel(proxyModel);

    m_scanArgs->config = m_scanConfig;

    this->initConfigValues();
}
Ssl::~Ssl(){
    delete m_model_hash;
    delete m_model_ssl;
    delete m_model_subdomain;
    delete m_targetListModel;
    delete m_scanStats;
    delete m_scanArgs;
    delete m_scanConfig;
    delete ui;
}

void Ssl::initUI(){
    ui->setupUi(this);

    /* setting widget's properties */
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);
    ui->labelOUT->setProperty("s3s_color", true);

    /* enabling and disabling widgets */
    ui->progressBar->hide();
    ui->comboBoxOption->hide();
    ui->buttonStop->setDisabled(true);

    /* placeholdertxt */
    ui->lineEditFilter->setPlaceholderText(tr("filter..."));
    ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_DOMAIN));

    /* equally seperate the widgets */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
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
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Target for Enumeration!"));
            return;
        }
        if(ui->checkBoxMultipleTargets->isChecked() && m_targetListModel->rowCount() < 1){
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Targets for Enumeration!"));
            return;
        }

        /* clear */
        m_failedScans.clear();
        m_scanArgs->targets.clear();

        /* get targets */
        if(ui->checkBoxMultipleTargets->isChecked()){
            foreach(const QString &target, m_targetListModel->stringList())
                m_scanArgs->targets.enqueue(target);
        }else {
            m_scanArgs->targets.enqueue(ui->lineEditTarget->text());
        }

        /* start scan */
        this->startScan();

        /* logs */
        log("------------------ start ----------------");
        qInfo() << "[SSL] Scan Started";
        return;
    }
    ///
    /// Pause scan...
    ///
    if(status->isRunning){
        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText(tr("Resume"));

        status->isPaused = true;
        status->isRunning = false;
        status->isStopped = false;
        status->isNotActive = false;

        /* pause scan */
        emit pauseScanThread();

        /* logs */
        log("------------------ Paused ----------------");
        qInfo() << "[SSL] Scan Paused";
        return;
    }
    ///
    /// Resume scan...
    ///
    if(status->isPaused){
        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText(tr("Pause"));

        status->isRunning = true;
        status->isPaused = false;
        status->isStopped = false;
        status->isNotActive = false;

        /* resume scan */
        emit resumeScanThread();

        /* logs */
        log("------------------ Resumed ----------------");
        qInfo() << "[SSL] Scan Resumed";
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
        proxyModel->setSourceModel(m_model_subdomain);
        ui->comboBoxOption->hide();
        ui->labelInfo->setText(tr("Enumerated Subdomains:"));
        ui->treeViewResults->setIndentation(0);
        break;
    case 1: // cert hash
        proxyModel->setSourceModel(m_model_hash);
        ui->comboBoxOption->show();
        ui->labelInfo->setText(tr("Enumerated Certificates:"));
        ui->treeViewResults->setIndentation(0);
        break;
    case 2: // ssl cert
        proxyModel->setSourceModel(m_model_ssl);
        ui->comboBoxOption->hide();
        ui->labelInfo->setText(tr("Enumerated Certificates:"));
        ui->treeViewResults->setIndentation(20);
        break;
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Ssl::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Ssl::on_lineEditFilter_textChanged(const QString &filterKeyword){
    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Ssl::initConfigValues(){
    CONFIG.beginGroup(CFG_SSL);
    m_scanArgs->config->timeout = CONFIG.value(CFG_VAL_TIMEOUT).toInt();
    m_scanArgs->config->threads = CONFIG.value(CFG_VAL_THREADS).toInt();
    m_scanArgs->config->noDuplicates = CONFIG.value(CFG_VAL_DUPLICATES).toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG.value(CFG_VAL_AUTOSAVE).toBool();
    CONFIG.endGroup();
}

void Ssl::log(const QString &log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Ssl::on_treeViewResults_clicked(const QModelIndex &index){
    if(ui->comboBoxOutput->currentIndex() == 2){
        QModelIndex model_index = proxyModel->mapToSource(index);
        if(model_index.parent() == m_model_ssl->invisibleRootItem()->index()){
            s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model_ssl->itemFromIndex(model_index));
            ui->plainTextEditCert->setPlainText(item->raw);
            ui->plainTextEditKey->setPlainText(item->raw_key);
        }
    }
}
