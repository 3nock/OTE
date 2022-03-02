/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "SSLEnum.h"
#include "ui_SSLEnum.h"

#include "src/utils/Config.h"
#include "src/dialogs/EnumConfigDialog.h"


SSLEnum::SSLEnum(QWidget *parent, ProjectModel *project) : AbstractEnum(parent, project),
    ui(new Ui::SSLEnum),
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
    m_model->setHorizontalHeaderLabels({tr(" SSL"), tr(" Values")});
    proxyModel->setSourceModel(m_model);
    ui->treeViewResults->setModel(proxyModel);

    /* scan arguments */
    m_scanArgs->config = m_scanConfig;
}
SSLEnum::~SSLEnum(){
    delete m_scanArgs;
    delete m_scanConfig;
    delete m_targetsListModel;
    delete m_model;
    delete ui;
}

void SSLEnum::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void SSLEnum::on_buttonStart_clicked(){
    /* checks */
    if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Target for Enumeration!"));
        return;
    }
    if(ui->checkBoxMultipleTargets->isChecked() && m_targetsListModel->rowCount() < 1){
        QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Targets for Enumeration!"));
        return;
    }

    /* enabling/disabling widgets... */
    ui->buttonStop->setEnabled(true);
    ui->buttonStart->setDisabled(true);

    /* setting status */
    status->isRunning = true;
    status->isNotActive = false;
    status->isStopped = false;
    status->isPaused = false;

    /* start scan */
    this->startScan();

    /* logs */
    this->log("------------------ start ----------------");
    qInfo() << "[SSL-Enum] Scan Started";
}

void SSLEnum::on_buttonStop_clicked(){
    emit stopScanThread();

    status->isStopped = true;
    status->isNotActive = false;
    status->isPaused = false;
    status->isRunning = false;
}

void SSLEnum::on_buttonConfig_clicked(){
    EnumConfigDialog *scanConfig = new EnumConfigDialog(this, m_scanConfig);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->loadConfig_ssl();
    scanConfig->show();
}

void SSLEnum::initUI(){
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
    ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_SSLCERT));

    /* resizing splitter */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                           << static_cast<int>((this->width() * 0.50)));
}

void SSLEnum::initConfigValues(){
    m_scanConfig->autosaveToProject = CONFIG_ENUM.value("autosave_to_Project_ssl").toBool();
    m_scanConfig->noDuplicates = CONFIG_ENUM.value("no_duplicates_ssl").toBool();
}

void SSLEnum::log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void SSLEnum::on_lineEditFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void SSLEnum::on_treeViewResults_clicked(const QModelIndex &index){
    QModelIndex model_index = proxyModel->mapToSource(index);
    if(model_index.parent() == m_model->invisibleRootItem()->index()){
        s3s_item::SSL *item = static_cast<s3s_item::SSL*>(m_model->itemFromIndex(model_index));
        ui->plainTextEditRawCert->setPlainText(item->raw);
        ui->plainTextEditRawKey->setPlainText(item->raw_key);
    }
}
