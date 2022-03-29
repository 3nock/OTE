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
#include "src/utils/utils.h"
#include "src/dialogs/ActiveConfigDialog.h"

/*
 * TODO:
 *      insert the send/copy/save targets to set before appending to clipboard or file
 */
Url::Url(QWidget *parent, ProjectModel *project) : AbstractEngine(parent, project),
    ui(new Ui::Url),
    m_scanConfig(new url::ScanConfig),
    m_scanArgs(new url::ScanArgs),
    m_scanStats(new url::ScanStat),
    m_targetListModel(new QStringListModel),
    m_model(new QStandardItemModel)
{
    this->initUI();

    /* result model */
    ui->tableViewResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_model->setHorizontalHeaderLabels({tr(" URL"), tr(" Status"), tr(" Server"), tr(" Content Type")});
    proxyModel->setSourceModel(m_model);
    ui->tableViewResults->setModel(proxyModel);

    ui->tableViewResults->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    /* config... */
    m_scanArgs->config = m_scanConfig;

    /* get prev config values */
    this->initConfigValues();
}
Url::~Url(){
    delete m_model;
    delete m_targetListModel;
    delete m_scanStats;
    delete m_scanArgs;
    delete m_scanConfig;
    delete ui;
}

void Url::initUI(){
    ui->setupUi(this);

    /* widget properties */
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);

    /* targets */
    ui->targets->setListName(tr("Targets"));
    ui->targets->setListModel(m_targetListModel);

    /* hiding widgets */
    ui->progressBar->hide();
    ui->buttonStop->setDisabled(true);

    /* placeholdertext */
    ui->lineEditFilter->setPlaceholderText(tr("filter..."));
    ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_DOMAIN));

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
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
        qInfo() << "[URL] Scan Started";
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
        qInfo() << "[URL] Scan Paused";
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
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Url::initConfigValues(){
    CONFIG.beginGroup(CFG_URL);
    m_scanArgs->config->timeout = CONFIG.value(CFG_VAL_TIMEOUT).toInt();
    m_scanArgs->config->threads = CONFIG.value(CFG_VAL_THREADS).toInt();
    m_scanArgs->config->noDuplicates = CONFIG.value(CFG_VAL_DUPLICATES).toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG.value(CFG_VAL_AUTOSAVE).toBool();
    m_scanArgs->config->setTimeout = CONFIG.value(CFG_VAL_SETTIMEOUT).toBool();
    CONFIG.endGroup();
}

void Url::log(const QString &log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Url::on_lineEditFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->tableViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}
