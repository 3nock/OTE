/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "Url.h"
#include "ui_Url.h"

#include <QThread>
#include <QDateTime>
#include "src/utils/Config.h"
#include "src/utils/Definitions.h"
#include "src/dialogs/ActiveConfigDialog.h"


Url::Url(QWidget *parent, ProjectModel *project) : AbstractEngine(parent, project),
    ui(new Ui::Url),
    m_scanConfig(new url::ScanConfig),
    m_scanArgs(new url::ScanArgs),
    m_scanStats(new url::ScanStat),
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
    m_resultModel->setHorizontalHeaderLabels({" URL", " Status", " Server", " Content Type"});
    m_resultProxyModel->setSourceModel(m_resultModel);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->tableViewResults->setModel(m_resultProxyModel);

    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableViewResults->horizontalHeader()->resizeSection(0, 230);
    ui->tableViewResults->horizontalHeader()->resizeSection(1, 10);
    ui->tableViewResults->horizontalHeader()->resizeSection(2, 120);

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
Url::~Url(){
    delete m_resultProxyModel;
    delete m_resultModel;
    delete m_targetListModel;
    delete m_scanStats;
    delete m_scanArgs;
    delete m_scanConfig;
    delete ui;
}

void Url::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void Url::on_buttonStart_clicked(){
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
        qInfo() << "[URL] Scan Started";
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
        qInfo() << "[URL] Scan Paused";
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
        qInfo() << "[URL] Scan Resumed";
    }
}

void Url::on_buttonStop_clicked(){
    if(status->isPaused)
        emit resumeScanThread();

    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void Url::on_buttonConfig_clicked(){
    /*
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
    */
}

void Url::m_getConfigValues(){
    m_scanArgs->config->timeout = CONFIG_ACTIVE.value("timeout").toInt();
    m_scanArgs->config->threads = CONFIG_ACTIVE.value("threads").toInt();
    m_scanArgs->config->noDuplicates = CONFIG_ACTIVE.value("noDuplicates").toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG_ACTIVE.value("autosaveToProject").toBool();
}

void Url::m_log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Url::on_lineEditFilter_textChanged(const QString &filterKeyword){
    m_resultProxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        m_resultProxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        m_resultProxyModel->setFilterFixedString(filterKeyword);

    ui->tableViewResults->setModel(m_resultProxyModel);
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}
