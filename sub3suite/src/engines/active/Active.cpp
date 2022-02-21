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


Active::Active(QWidget *parent, ProjectModel *project) : AbstractEngine(parent, project),
    ui(new Ui::Active),
    m_scanConfig(new active::ScanConfig),
    m_scanArgs(new active::ScanArgs),
    m_scanStats(new active::ScanStat),
    m_targetListModel(new QStringListModel),
    m_resultModel(new QStandardItemModel),
    m_resultProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);

    /* targets */
    ui->targets->setListName("Targets");
    ui->targets->setListModel(m_targetListModel);

    /* result model */
    m_resultModel->setHorizontalHeaderLabels({" Host", " Ipv4", " Ipv6", " Ports"});
    m_resultProxyModel->setSourceModel(m_resultModel);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->tableViewResults->setModel(m_resultProxyModel);

    ui->tableViewResults->horizontalHeader()->resizeSection(0, 200);
    ui->tableViewResults->horizontalHeader()->resizeSection(1, 100);
    ui->tableViewResults->horizontalHeader()->resizeSection(2, 200);
    ui->tableViewResults->horizontalHeader()->resizeSection(3, 100);
    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    /* hiding widgets */
    ui->progressBar->hide();
    ui->buttonStop->setDisabled(true);

    /* equally seperate the widgets... */
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
    delete m_resultProxyModel;
    delete m_resultModel;
    delete m_targetListModel;
    delete m_scanStats;
    delete m_scanArgs;
    delete m_scanConfig;
    delete ui;
}

void Active::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void Active::on_buttonStart_clicked(){
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

void Active::on_buttonStop_clicked(){
    if(status->isPaused)
        emit resumeScanThread();

    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
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
    if(record == "ANY")
        m_scanArgs->config->recordType = QDnsLookup::ANY;

    int size = CONFIG_ACTIVE.beginReadArray("Nameservers");
    for (int i = 0; i < size; ++i) {
        CONFIG_ACTIVE.setArrayIndex(i);
        m_scanArgs->config->nameservers.append(CONFIG_ACTIVE.value("value").toString());
    }
    CONFIG_ACTIVE.endArray();
}

void Active::m_log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Active::on_lineEditFilter_textChanged(const QString &filterKeyword){
    m_resultProxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        m_resultProxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        m_resultProxyModel->setFilterFixedString(filterKeyword);

    ui->tableViewResults->setModel(m_resultProxyModel);
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}
