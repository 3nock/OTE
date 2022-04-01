/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "MXEnum.h"
#include "ui_MXEnum.h"

#include "src/utils/Config.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/EnumConfigDialog.h"


MXEnum::MXEnum(QWidget *parent, ProjectModel *project) : AbstractEnum(parent, project),
    ui(new Ui::MXEnum),
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
    m_model->setHorizontalHeaderLabels({tr(" MX"), tr(" Values")});
    proxyModel->setSourceModel(m_model);
    ui->treeViewResults->setModel(proxyModel);

    /* scan arguments */
    m_scanArgs->config = m_scanConfig;
}
MXEnum::~MXEnum(){
    delete m_scanArgs;
    delete m_scanConfig;
    delete m_targetsListModel;
    delete m_model;
    delete ui;
}

void MXEnum::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void MXEnum::on_buttonStart_clicked(){
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
    qInfo() << "[MX-Enum] Scan Started";
}

void MXEnum::on_buttonStop_clicked(){
    emit stopScanThread();

    status->isStopped = true;
    status->isNotActive = false;
    status->isPaused = false;
    status->isRunning = false;
}

void MXEnum::on_buttonConfig_clicked(){
    EnumConfigDialog *scanConfig = new EnumConfigDialog(this, m_scanConfig);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->loadConfig_mx();
    scanConfig->show();
}

void MXEnum::initUI(){
    ui->setupUi(this);

    /* hiding & disabling some widgets */
    ui->progressBar->hide();
    ui->buttonStop->setDisabled(true);

    /* setting specific properties */
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);
    ui->labelOut->setProperty("s3s_color", true);
    ui->labelModule->setProperty("s3s_color", true);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText(tr("Filter..."));
    ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_MX));

    /* resizing splitter */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                           << static_cast<int>((this->width() * 0.50)));
}

void MXEnum::initConfigValues(){
    CONFIG.beginGroup(CFG_ENUM);
    m_scanConfig->autosaveToProject = CONFIG.value("autosave_to_Project_mx").toBool();
    m_scanConfig->noDuplicates = CONFIG.value("no_duplicates_mx").toBool();
    m_scanConfig->setTimeout = CONFIG.value("set_timeout_mx").toBool();
    m_scanConfig->timeout = CONFIG.value("timeout_mx").toInt();
    CONFIG.endGroup();
}

void MXEnum::log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void MXEnum::on_lineEditFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void MXEnum::on_buttoApiKeys_clicked(){
    ApiKeysDialog *apiKeysDialog = new ApiKeysDialog(this);
    apiKeysDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeysDialog->show();
}
