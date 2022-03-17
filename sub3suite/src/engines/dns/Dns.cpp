/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Dns.h"
#include "ui_Dns.h"

#include <QDateTime>
#include <QClipboard>
#include "src/utils/Config.h"
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/utils/utils.h"


Dns::Dns(QWidget *parent, ProjectModel *project) : AbstractEngine(parent, project),
    ui(new Ui::Dns),
    m_scanConfig(new dns::ScanConfig),
    m_scanArgs(new dns::ScanArgs),
    m_scanStats(new dns::ScanStat),
    m_targetListModel(new QStringListModel),
    m_srvWordlitsModel(new QStringListModel),
    m_model(new QStandardItemModel)
{
    this->initUI();

    /* list model */
    ui->targets->setListName(tr("Targets"));
    ui->srvWordlist->setListName(tr("SRV"));
    ui->targets->setListModel(m_targetListModel);
    ui->srvWordlist->setListModel(m_srvWordlitsModel);

    /* result model */
    ui->treeViewResults->setHeaderHidden(false);
    m_model->setHorizontalHeaderLabels({tr(" DNS"), tr(" Values")});
    proxyModel->setSourceModel(m_model);
    ui->treeViewResults->setModel(proxyModel);

    m_scanArgs->config = m_scanConfig;

    this->initSrvWordlist();
    this->initConfigValues();
}
Dns::~Dns(){
    delete m_model;
    delete m_srvWordlitsModel;
    delete m_targetListModel;
    delete m_scanArgs;
    delete m_scanStats;
    delete m_scanConfig;
    delete ui;
}

void Dns::initUI(){
    ui->setupUi(this);

    /* setting widget's properties */
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);

    /* hiddin & disabling widgets */
    ui->buttonStop->setDisabled(true);
    ui->srvWordlist->hide();
    ui->progressBar->hide();

    /* placeholder texts */
    ui->lineEditFilter->setPlaceholderText("filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);

    /* equally seperate the widgets */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
}

void Dns::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void Dns::on_buttonStart_clicked(){
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
        if((!ui->checkBoxSRV->isChecked()) && (!ui->checkBoxANY->isChecked() && !ui->checkBoxA->isChecked() && !ui->checkBoxAAAA->isChecked() && !ui->checkBoxMX->isChecked() && !ui->checkBoxNS->isChecked() && !ui->checkBoxTXT->isChecked() && !ui->checkBoxCNAME->isChecked())){
            QMessageBox::warning(this, tr("Error!"), tr("Please Choose DNS Record To Enumerate!"));
            return;
        }
        if((ui->checkBoxSRV->isChecked())&& (m_srvWordlitsModel->rowCount() < 1)){
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter SRV Wordlist For Enumeration!"));
            return;
        }

        /* clear */
        m_failedScans.clear();
        m_scanArgs->targets.clear();
        m_scanArgs->srvWordlist.clear();

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
        qInfo() << "[DNS] Scan Started";
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
        qInfo() << "[DNS] Scan Paused";
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
        qInfo() << "[DNS] Scan Resumed";
    }
}

void Dns::on_buttonStop_clicked(){
    if(status->isPaused)
        emit resumeScanThread();

    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void Dns::initSrvWordlist(){
    QFile file(":/files/res/files/srv.txt");
    ui->srvWordlist->add(file);
}

void Dns::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Dns::on_checkBoxSRV_clicked(bool checked){
    if(checked)
        ui->srvWordlist->show();
    else
        ui->srvWordlist->hide();
}

void Dns::initConfigValues(){
    CONFIG.beginGroup(CFG_DNS);
    m_scanArgs->config->threads = CONFIG.value(CFG_VAL_THREADS).toInt();
    m_scanArgs->config->timeout = CONFIG.value(CFG_VAL_TIMEOUT).toInt();
    m_scanArgs->config->noDuplicates = CONFIG.value(CFG_VAL_DUPLICATES).toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG.value(CFG_VAL_AUTOSAVE).toBool();
    CONFIG.endGroup();

    int size = CONFIG.beginReadArray(CFG_ARR_NAMESERVERS);
    for (int i = 0; i < size; ++i) {
        CONFIG.setArrayIndex(i);
        m_scanArgs->config->nameservers.enqueue(CONFIG.value("value").toString());
    }
    CONFIG.endArray();
}

void Dns::log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Dns::on_lineEditFilter_textChanged(const QString &filterKeyword){
    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Dns::on_checkBoxANY_toggled(bool checked){
    if(checked){
        ui->checkBoxA->setChecked(false);
        ui->checkBoxMX->setChecked(false);
        ui->checkBoxNS->setChecked(false);
        ui->checkBoxSRV->setChecked(false);
        ui->checkBoxTXT->setChecked(false);
        ui->checkBoxAAAA->setChecked(false);
        ui->checkBoxCNAME->setChecked(false);
    }
    ui->srvWordlist->hide();
}

void Dns::on_checkBoxSRV_toggled(bool checked){
    if(checked){
        ui->checkBoxA->setChecked(false);
        ui->checkBoxMX->setChecked(false);
        ui->checkBoxNS->setChecked(false);
        ui->checkBoxANY->setChecked(false);
        ui->checkBoxTXT->setChecked(false);
        ui->checkBoxAAAA->setChecked(false);
        ui->checkBoxCNAME->setChecked(false);
    }
}
