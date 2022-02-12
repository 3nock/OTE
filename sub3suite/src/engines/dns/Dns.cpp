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
#include "src/utils/Definitions.h"

/*
 * save targets as json
 */
Dns::Dns(QWidget *parent, ProjectModel *project) : AbstractEngine(parent, project),
    ui(new Ui::Dns),
    m_scanConfig(new dns::ScanConfig),
    m_scanArgs(new dns::ScanArgs),
    m_scanStats(new dns::ScanStat),
    m_targetListModel(new QStringListModel),
    m_srvWordlitsModel(new QStringListModel),
    m_resultModel(new QStandardItemModel),
    m_resultProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);

    /* init... */
    ui->targets->setListName("Targets");
    ui->srvWordlist->setListName("SRV");
    ui->targets->setListModel(m_targetListModel);
    ui->srvWordlist->setListModel(m_srvWordlitsModel);

    /* result model */
    ui->treeViewResults->setHeaderHidden(false);
    m_resultModel->setHorizontalHeaderLabels({" DNS", " Values"});
    m_resultProxyModel->setSourceModel(m_resultModel);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->treeViewResults->setModel(m_resultProxyModel);


    /* widgets... */
    ui->buttonStop->setDisabled(true);
    ui->srvWordlist->hide();
    ui->progressBar->hide();

    /* placeholder texts */
    ui->lineEditFilter->setPlaceholderText("filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));

    m_scanArgs->config = m_scanConfig;

    /* init */
    this->m_initActions();
    this->m_loadSrvWordlist();
    this->m_getConfigValues();
}
Dns::~Dns(){
    delete m_resultProxyModel;
    delete m_resultModel;
    delete m_srvWordlitsModel;
    delete m_targetListModel;
    delete m_scanArgs;
    delete m_scanStats;
    delete m_scanConfig;
    delete ui;
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
            QMessageBox::warning(this, "Error!", "Please Enter the Target for Enumeration!");
            return;
        }
        if(ui->checkBoxMultipleTargets->isChecked() && m_targetListModel->rowCount() < 1){
            QMessageBox::warning(this, "Error!", "Please Enter the Targets for Enumeration!");
            return;
        }
        if((!ui->checkBoxSRV->isChecked()) && (!ui->checkBoxA->isChecked() && !ui->checkBoxAAAA->isChecked() && !ui->checkBoxMX->isChecked() && !ui->checkBoxNS->isChecked() && !ui->checkBoxTXT->isChecked() && !ui->checkBoxCNAME->isChecked())){
            QMessageBox::warning(this, "Error!", "Please Choose DNS Record To Enumerate!");
            return;
        }
        if((ui->checkBoxSRV->isChecked())&& (m_srvWordlitsModel->rowCount() < 1)){
            QMessageBox::warning(this, "Error!", "Please Enter SRV Wordlist For Enumeration!");
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

void Dns::on_buttonStop_clicked(){
    if(status->isPaused)
        emit resumeScanThread();

    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void Dns::m_loadSrvWordlist(){
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

void Dns::m_getConfigValues(){
    m_scanArgs->config->threads = CONFIG_DNS.value("threads").toInt();
    m_scanArgs->config->noDuplicates = CONFIG_DNS.value("noDuplicates").toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG_DNS.value("autosaveToProject").toBool();

    int size = CONFIG_DNS.beginReadArray("Nameservers");
    for (int i = 0; i < size; ++i) {
        CONFIG_DNS.setArrayIndex(i);
        m_scanArgs->config->nameservers.append(CONFIG_DNS.value("value").toString());
    }
    CONFIG_DNS.endArray();
}

void Dns::m_log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Dns::on_lineEditFilter_textChanged(const QString &filterKeyword){
    if(ui->checkBoxRegex->isChecked())
        m_resultProxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        m_resultProxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(m_resultProxyModel);
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}
