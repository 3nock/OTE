/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "HostTool.h"
#include "ui_HostTool.h"

#include <QThread>
#include <QDateTime>
#include "src/utils/Config.h"
#include "src/utils/utils.h"
#include "src/dialogs/ActiveConfigDialog.h"


HostTool::HostTool(QWidget *parent, ProjectModel *project) : AbstractTool(parent, project),
    ui(new Ui::HostTool),
    m_scanConfig(new host::ScanConfig),
    m_scanArgs(new host::ScanArgs),
    m_scanStats(new host::ScanStat),
    m_portscannerArgs(new port::ScanArgs),
    m_pingscannerArgs(new ping::ScanArgs),
    m_targetListModel(new QStringListModel),
    m_model_dns(new QStandardItemModel),
    m_model_port(new QStandardItemModel),
    m_model_ping(new QStandardItemModel)
{
    this->initUI();

    /* list model */
    ui->targets->setListName(tr("Targets"));
    ui->targets->setListModel(m_targetListModel);

    /* result model */
    m_model_ping->setHorizontalHeaderLabels({tr(" Host"), tr(" IP"), tr(" Time(ms)")});
    m_model_port->setHorizontalHeaderLabels({tr(" Host"), tr(" IP"), tr(" Ports")});
    m_model_dns->setHorizontalHeaderLabels({tr(" Host"), tr(" IPv4"), tr(" IPv6")});
    proxyModel->setSourceModel(m_model_dns);
    ui->tableViewResults->setModel(proxyModel);

    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    /* config... */
    m_scanConfig->portScanConfig = m_portscannerArgs;
    m_scanArgs->config = m_scanConfig;
    m_pingscannerArgs->is_host = true;
    m_portscannerArgs->is_host = true;

    this->initConfigValues();
}
HostTool::~HostTool(){
    delete m_model_ping;
    delete m_model_port;
    delete m_model_dns;
    delete m_targetListModel;
    delete m_pingscannerArgs;
    delete m_portscannerArgs;
    delete m_scanStats;
    delete m_scanArgs;
    delete m_scanConfig;
    delete ui;
}

void HostTool::initUI(){
    ui->setupUi(this);

    /* widgets properties */
    ui->frame->setProperty("default_frame", true);
    ui->framePort->setProperty("dark_frame",true);
    ui->labelResultsCount->setProperty("dark", true);

    /* hiding widgets */
    ui->framePort->hide();
    ui->progressBar->hide();
    ui->buttonStop->setDisabled(true);

    /* placeholdertext */
    ui->lineEditFilter->setPlaceholderText(tr("filter..."));
    ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_DOMAIN));

    ui->lineEditCustom->setPlaceholderText(tr("e.g. 80,443,21"));
    ui->lineEditFrom->setPlaceholderText(tr("e.g. 1"));
    ui->lineEditTo->setPlaceholderText(tr("e.g. 1023"));

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}

void HostTool::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void HostTool::on_buttonStart_clicked(){
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
        qInfo() << "[ACTIVE] Scan Started";
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
        qInfo() << "[ACTIVE] Scan Paused";
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
        qInfo() << "[ACTIVE] Scan Resumed";
    }
}

void HostTool::on_buttonStop_clicked(){
    if(status->isPaused)
        emit resumeScanThread();

    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void HostTool::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void HostTool::initConfigValues(){
    CONFIG.beginGroup(CFG_ACTIVE);
    m_scanArgs->config->timeout = CONFIG.value(CFG_VAL_TIMEOUT).toInt();
    m_scanArgs->config->threads = CONFIG.value(CFG_VAL_THREADS).toInt();
    m_scanArgs->config->noDuplicates = CONFIG.value(CFG_VAL_DUPLICATES).toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG.value(CFG_VAL_AUTOSAVE).toBool();
    m_scanArgs->config->setTimeout = CONFIG.value(CFG_VAL_SETTIMEOUT).toBool();
    QString record = CONFIG.value(CFG_VAL_RECORD).toString();
    QString portScan = CONFIG.value(CFG_VAL_PORTSCAN).toString();
    CONFIG.endGroup();

    if(portScan == "syn")
        m_portscannerArgs->scan_type = port::ScanType::SYN;
    if(portScan == "connection")
        m_portscannerArgs->scan_type = port::ScanType::CONNECTION;

    if(record == "A")
        m_scanArgs->config->recordType = QDnsLookup::A;
    if(record == "AAAA")
        m_scanArgs->config->recordType = QDnsLookup::AAAA;
    if(record == "ANY")
        m_scanArgs->config->recordType = QDnsLookup::ANY;

    int size = CONFIG.beginReadArray("nameservers_active");
    for (int i = 0; i < size; ++i) {
        CONFIG.setArrayIndex(i);
        m_scanArgs->config->nameservers.enqueue(CONFIG.value("value").toString());
    }
    CONFIG.endArray();
}

void HostTool::log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void HostTool::on_lineEditFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->tableViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void HostTool::on_comboBoxOption_currentIndexChanged(int index){
    switch (index) {
    case 0: // ACTIVE DNS
        ui->framePort->hide();
        proxyModel->setSourceModel(m_model_dns);
        break;
    case 1: // ACTIVE PORT]
        if(!s3s_global::is_priv){
#if defined (Q_OS_WIN)
            QMessageBox::warning(this, "Warning!", "Please run Sub3 Suite as Administrator to use this feature!");
#else
            QMessageBox::warning(this, "Warning!", "Please run Sub3 Suite with root permission to use this feature!");
#endif
            ui->comboBoxOption->setCurrentIndex(0);
            return;
        }
        ui->framePort->show();
        proxyModel->setSourceModel(m_model_port);
        break;
    case 2: // ACTIVE PING
        if(!s3s_global::is_priv){
#if defined (Q_OS_WIN)
            QMessageBox::warning(this, "Warning!", "Please run Sub3 Suite as Administrator to use this feature!");
#else
            QMessageBox::warning(this, "Warning!", "Please run Sub3 Suite with root permission to use this feature!");
#endif
            ui->comboBoxOption->setCurrentIndex(0);
            return;
        }
        ui->framePort->hide();
        proxyModel->setSourceModel(m_model_ping);
        break;
    }
    ui->tableViewResults->setModel(proxyModel);
}
