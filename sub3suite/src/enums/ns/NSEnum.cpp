/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "NSEnum.h"
#include "ui_NSEnum.h"

#include "src/utils/Config.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/EnumConfigDialog.h"


NSEnum::NSEnum(QWidget *parent, ProjectModel *project) : AbstractEnum(parent, project),
    ui(new Ui::NSEnum),
    m_model(new QStandardItemModel),
    m_targetsListModel(new QStringListModel),
    m_scanConfig(new ScanConfig),
    m_scanArgs(new ScanArgs)
{
    this->initUI();
    this->initConfigValues();

    /* setting targets model */
    ui->targets->setListName("Targets");
    ui->targets->setListModel(m_targetsListModel);

    /* setting model with tableView... */
    m_model->setHorizontalHeaderLabels({tr(" NS"), tr(" Values")});
    proxyModel->setSourceModel(m_model);
    ui->treeViewResults->setModel(proxyModel);

    /* scan arguments */
    m_scanArgs->config = m_scanConfig;
}
NSEnum::~NSEnum(){
    delete m_scanArgs;
    delete m_scanConfig;
    delete m_targetsListModel;
    delete m_model;
    delete ui;
}

void NSEnum::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void NSEnum::on_buttonStart_clicked(){
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
    qInfo() << "[NS-Enum] Scan Started";
}

void NSEnum::on_buttonStop_clicked(){
    emit stopScanThread();

    status->isStopped = true;
    status->isNotActive = false;
    status->isPaused = false;
    status->isRunning = false;
}

void NSEnum::on_buttonConfig_clicked(){
    EnumConfigDialog *scanConfig = new EnumConfigDialog(this, m_scanConfig);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->loadConfig_ns();
    scanConfig->show();
}

void NSEnum::initUI(){
    ui->setupUi(this);

    /* hiding & disabling some widgets */
    ui->progressBar->hide();
    ui->buttonStop->setDisabled(true);

    /* setting specific properties */
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);
    ui->labelModule->setProperty("s3s_color", true);

    /* placeholder texts... */
    ui->lineEditFilter->setPlaceholderText(tr("Filter..."));
    ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_NS));

    /* resizing splitter */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                           << static_cast<int>((this->width() * 0.50)));
}

void NSEnum::initConfigValues(){
    CONFIG.beginGroup(CFG_ENUM);
    m_scanConfig->autosaveToProject = CONFIG.value("autosave_to_Project_ns").toBool();
    m_scanConfig->noDuplicates = CONFIG.value("no_duplicates_ns").toBool();
    m_scanConfig->setTimeout = CONFIG.value("set_timeout_ns").toBool();
    m_scanConfig->timeout = CONFIG.value("timeout_ns").toInt();
    CONFIG.endGroup();
}

void NSEnum::log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void NSEnum::on_lineEditFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void NSEnum::on_buttoApiKeys_clicked(){
    ApiKeysDialog *apiKeysDialog = new ApiKeysDialog(this);
    apiKeysDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeysDialog->show();
}
