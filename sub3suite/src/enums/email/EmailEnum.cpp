/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "EmailEnum.h"
#include "ui_EmailEnum.h"

#include "src/utils/Config.h"
#include "src/dialogs/EnumConfigDialog.h"


EmailEnum::EmailEnum(QWidget *parent, ProjectModel *project) : AbstractEnum(parent, project),
    ui(new Ui::EmailEnum),
    m_model(new QStandardItemModel),
    m_targetsListModel(new QStringListModel),
    m_scanConfig(new ScanConfig),
    m_scanArgs(new ScanArgs)
{
    this->initUI();
    this->initConfigValues();

    /* setting targets model */
    ui->targets->setListName(tr("Targets"));
    ui->targets->setListModel(m_targetsListModel);

    /* setting model with tableView... */
    m_model->setHorizontalHeaderLabels({tr(" Email"), tr(" Values")});
    proxyModel->setSourceModel(m_model);
    ui->treeViewResults->setModel(proxyModel);

    /* scan arguments */
    m_scanArgs->config = m_scanConfig;
}
EmailEnum::~EmailEnum(){
    delete m_scanArgs;
    delete m_scanConfig;
    delete m_targetsListModel;
    delete m_model;
    delete ui;
}

void EmailEnum::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void EmailEnum::on_buttonStart_clicked(){
    /* checks */
    if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Target for Enumeration!"));
        return;
    }
    if(ui->checkBoxMultipleTargets->isChecked() && m_targetsListModel->rowCount() < 1){
        QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Targets for Enumeration!"));
        return;
    }

    /* getting targets */
    m_scanArgs->targets.clear();
    if(ui->checkBoxMultipleTargets->isChecked()){
        foreach(const QString &target, m_targetsListModel->stringList())
            m_scanArgs->targets.enqueue(target);
    }else{
        m_scanArgs->targets.enqueue(ui->lineEditTarget->text());
    }

    /* start scan */
    this->startScan();

    /* logs */
    this->log("------------------ start ----------------");
    qInfo() << "[Email-Enum] Scan Started";
}

void EmailEnum::on_buttonStop_clicked(){
    emit stopScanThread();

    status->isStopped = true;
    status->isNotActive = false;
    status->isPaused = false;
    status->isRunning = false;
}

void EmailEnum::on_buttonConfig_clicked(){
    EnumConfigDialog *scanConfig = new EnumConfigDialog(this, m_scanConfig);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->loadConfig_email();
    scanConfig->show();
}

void EmailEnum::initUI(){
    ui->setupUi(this);

    /* hiding & disabling some widgets */
    ui->progressBar->hide();
    ui->buttonStop->setDisabled(true);

    /* setting specific properties */
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);
    ui->labelModule->setProperty("s3s_color", true);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText("Filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_EMAIL);

    /* resizing splitter */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                           << static_cast<int>((this->width() * 0.50)));
}

void EmailEnum::initConfigValues(){
    CONFIG.beginGroup(CFG_ENUM);
    m_scanConfig->autosaveToProject = CONFIG.value("autosave_to_Project_email").toBool();
    m_scanConfig->noDuplicates = CONFIG.value("no_duplicates_email").toBool();
    m_scanConfig->timeout = CONFIG.value("timeout_email").toInt();
    CONFIG.endGroup();
}

void EmailEnum::log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void EmailEnum::on_lineEditFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}
