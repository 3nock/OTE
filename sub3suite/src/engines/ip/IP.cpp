/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "IP.h"
#include "ui_IP.h"

#include <QThread>
#include <QDateTime>
#include "src/utils/Config.h"
#include "src/utils/utils.h"
#include "src/dialogs/ActiveConfigDialog.h"


IP::IP(QWidget *parent, ProjectModel *project) : AbstractEngine(parent, project),
    ui(new Ui::IP),
    m_scanConfig(new ip::ScanConfig),
    m_scanArgs(new ip::ScanArgs),
    m_scanStats(new ip::ScanStat),
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
    m_model_ping->setHorizontalHeaderLabels({tr(" IP"), tr(" Bytes"), tr(" Time(ms)"), tr(" TTL")});
    m_model_port->setHorizontalHeaderLabels({tr(" IP"), tr(" Open Ports")});
    m_model_dns->setHorizontalHeaderLabels({tr(" IP"), tr(" Hostname")});
    proxyModel->setSourceModel(m_model_dns);
    ui->tableViewResults->setModel(proxyModel);

    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    /* config */
    m_scanArgs->config = m_scanConfig;
    m_pingscannerArgs->is_ip = true;
    m_portscannerArgs->is_ip = true;

    this->initConfigValues();
}
IP::~IP(){
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

void IP::initUI(){
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
    ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_IP));

    ui->lineEditCustom->setPlaceholderText(tr("e.g. 80,443,21"));
    ui->lineEditFrom->setPlaceholderText(tr("e.g. 1"));
    ui->lineEditTo->setPlaceholderText(tr("e.g. 1023"));

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}

void IP::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void IP::on_buttonStart_clicked(){
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

void IP::on_buttonStop_clicked(){
    if(status->isPaused)
        emit resumeScanThread();

    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void IP::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void IP::initConfigValues(){
    CONFIG.beginGroup(CFG_IP);
    m_scanArgs->config->timeout = CONFIG.value(CFG_VAL_TIMEOUT).toInt();
    m_scanArgs->config->threads = CONFIG.value(CFG_VAL_THREADS).toInt();
    m_scanArgs->config->noDuplicates = CONFIG.value(CFG_VAL_DUPLICATES).toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG.value(CFG_VAL_AUTOSAVE).toBool();
    m_scanArgs->config->setTimeout = CONFIG.value(CFG_VAL_SETTIMEOUT).toBool();
    CONFIG.endGroup();

    int size = CONFIG.beginReadArray("nameservers_active");
    for (int i = 0; i < size; ++i) {
        CONFIG.setArrayIndex(i);
        m_scanArgs->config->nameservers.enqueue(CONFIG.value("value").toString());
    }
    CONFIG.endArray();
}

void IP::log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void IP::on_lineEditFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->tableViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void IP::on_comboBoxOption_currentIndexChanged(int index){
    switch (index) {
    case 0: // REVERSE LOOKUP
        ui->framePort->hide();
        proxyModel->setSourceModel(m_model_dns);
        break;
    case 1: // PORT SCAN
        ui->framePort->show();
        proxyModel->setSourceModel(m_model_port);
        break;
    case 2: // PING SCAN
        ui->framePort->hide();
        proxyModel->setSourceModel(m_model_ping);
        break;
    }
    ui->tableViewResults->setModel(proxyModel);
}
