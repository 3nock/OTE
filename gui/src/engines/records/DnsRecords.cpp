#include "DnsRecords.h"
#include "ui_DnsRecords.h"
//...
#include <QDateTime>
#include <QClipboard>


DnsRecords::DnsRecords(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::DnsRecords),
    m_scanArguments(new records::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    ui->srvWordlist->init("SRV");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Records");
    ///
    /// other initializations...
    ///
    ui->buttonStop->setDisabled(true);
    //ui->buttonPause->setDisabled(true);
    //...
    ui->srvWordlist->hide();
    ui->progressBar->hide();
    ui->progressBarSRV->hide();
    //...
    ui->lineEditFilter->hide();
    ui->buttonFilter->hide();
    ui->comboBoxFilter->hide();
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ///
    /// equsubdomainIpy seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    result->records->dns->setHorizontalHeaderLabels({"Name", "Target", "Port"});
    //...
    ui->treeViewResults->setModel(result->records->dns);
    ui->tableViewSRV->setModel(result->records->srv);
    //...
    this->loadSrvWordlist();
    this->connectActions();
    //...
    qRegisterMetaType<records::Results>("records::Results");
    result->records->dnsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->records->dnsProxy->setRecursiveFilteringEnabled(true);
    result->records->srvProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->records->srvProxy->setRecursiveFilteringEnabled(true);
}
DnsRecords::~DnsRecords(){
    delete m_scanArguments;
    delete ui;
}

void DnsRecords::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void DnsRecords::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void DnsRecords::on_buttonStart_clicked(){
    ///
    /// checking if subdomainIp requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(ui->targets->listModel->rowCount() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS) && (!ui->checkBoxA->isChecked() && !ui->checkBoxAAAA->isChecked() && !ui->checkBoxMX->isChecked() && !ui->checkBoxNS->isChecked() && !ui->checkBoxTXT->isChecked() && !ui->checkBoxCNAME->isChecked())){
        QMessageBox::warning(this, "Error!", "Please Choose DNS Record To Enumerate!");
        return;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::SRV)&& (ui->srvWordlist->listModel->rowCount() < 1)){
        QMessageBox::warning(this, "Error!", "Please Enter SRV Wordlist For Enumeration!");
        return;
    }
    ///
    /// disabling & Enabling widgets...
    ///
    ui->buttonStart->setDisabled(true);
    //ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ///
    /// Resetting the scan arguments values...
    ///
    m_scanArguments->targetList = ui->targets->listModel->stringList();
    m_scanArguments->srvWordlist = ui->srvWordlist->listModel->stringList();
    m_scanArguments->currentSrvToEnumerate = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    ///
    /// getting the arguments for Dns Records Scan...
    ///
    if(ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS)
    {
        ui->progressBar->show();
        ui->progressBar->reset();
        m_scanArguments->progress = 0;
        //...
        m_scanArguments->RecordType_srv = false;
        m_scanArguments->RecordType_a = ui->checkBoxA->isChecked();
        m_scanArguments->RecordType_aaaa = ui->checkBoxAAAA->isChecked();
        m_scanArguments->RecordType_mx = ui->checkBoxMX->isChecked();
        m_scanArguments->RecordType_ns = ui->checkBoxNS->isChecked();
        m_scanArguments->RecordType_txt = ui->checkBoxTXT->isChecked();
        m_scanArguments->RecordType_cname = ui->checkBoxCNAME->isChecked();
        //...
        ui->progressBar->setMaximum(ui->targets->listModel->rowCount());
    }
    ///
    /// getting arguments for SRV DNS Records Scan...
    ///
    if(ui->comboBoxOption->currentIndex() == OPTION::SRV)
    {
        ui->progressBarSRV->show();
        ui->progressBarSRV->reset();
        m_scanArguments->progress = 0;
        m_scanArguments->RecordType_srv = true;
        ui->progressBarSRV->setMaximum(ui->targets->listModel->rowCount()*ui->srvWordlist->listModel->rowCount());
    }
    ///
    /// start Enumeration...
    ///
    startScan();
    //...
    sendStatus("[START] Started Subdomain Enumeration!");
}

void DnsRecords::on_buttonStop_clicked(){
    emit stopScan();
    status->records->isStopped = true;
}

void DnsRecords::loadSrvWordlist(){
    QFile file(":/files/res/files/srv.txt");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd()){
            ui->srvWordlist->add(in.readLine());
        }
        file.close();
    }
}

void DnsRecords::onShowFilter(bool show){
    if(show){
        ui->buttonFilter->show();
        ui->lineEditFilter->show();
        ui->comboBoxFilter->show();
    }
    else{
        ui->buttonFilter->hide();
        ui->lineEditFilter->hide();
        ui->comboBoxFilter->hide();
    }
}

void DnsRecords::onExpandResultsDnsRecords(){
    ui->treeViewResults->expandAll();
}

void DnsRecords::onCollapseResultsDnsRecords(){
    ui->treeViewResults->collapseAll();
}

void DnsRecords::onClearResultsDnsRecords(){
    ///
    /// clear the results...
    ///
    result->records->dns->clear();
    ui->labelResultsCount->clear();
    ///
    /// clear the progressbar...
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
}

void DnsRecords::onClearResultsSrvRecords(){
    ///
    /// clear the results...
    ///
    result->records->srv->clear();
    ui->labelResultsCountSRV->clear();
    result->records->srv->setHorizontalHeaderLabels({"Name", "Target", "Port"});
    ///
    /// hide the action button...
    ///
    ui->progressBarSRV->hide();
}

void DnsRecords::on_buttonConfig_clicked(){
    /*
    ScanConfig *bruteconfig = new ScanConfig(this, m_scanConfig, ENGINE::RECORDS);
    bruteconfig->setAttribute( Qt::WA_DeleteOnClose, true );
    bruteconfig->show();
    */
}

void DnsRecords::on_comboBoxOption_currentIndexChanged(int index){
    if(index)
    {
        ui->srvWordlist->show();
        ui->frame_records->hide();
        ui->tabWidgetResults->setCurrentIndex(1);
    }
    else
    {
        ui->srvWordlist->hide();
        ui->frame_records->show();
        ui->tabWidgetResults->setCurrentIndex(0);
    }
}

void DnsRecords::on_buttonFilter_clicked(){
    QString keyword = ui->lineEditFilter->text();
}
