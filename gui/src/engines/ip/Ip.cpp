#include "Ip.h"
#include "ui_Ip.h"
//...
#include <QDateTime>
#include <QClipboard>


/*
 * check if ip is active for https/ftp/ssh
 */

Ip::Ip(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Ip),
    m_scanArguments(new ipEngine::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Target Ip");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Ip");
    ///
    /// ...
    ///
    ui->progressBar->hide();
    //...
    ui->lineEditFilter->hide();
    ui->buttonFilter->hide();
    ui->comboBoxFilter->hide();
    //...
    ui->buttonStop->setDisabled(true);
    //ui->buttonPause->setDisabled(true);
    //...
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    result->ip->subdomainIp->setHorizontalHeaderLabels({"IpAddress:", "HostName:"});
    ui->tableViewResults->setModel(result->ip->subdomainIp);
    ///
    /// equsubdomainIpy seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    ///
    /// ...
    ///
    this->connectActions();
    result->ip->subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->ip->subdomainIpProxy->setRecursiveFilteringEnabled(true);
}
Ip::~Ip(){
    delete m_scanArguments;
    //...
    delete ui;
}

void Ip::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Ip::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Ip::on_buttonStart_clicked(){
    ///
    /// checking if subdomainIp requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(!(ui->targets->listModel->rowCount() > 0)){
        QMessageBox::warning(this, "Error!", "Please Enter the subdomains Wordlist for Enumeration!");
        return;
    }
    ///
    /// disabling and Enabling widgets...
    ///
    ui->buttonStart->setDisabled(true);
    //ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();
    ///
    /// Resetting the scan arguments values...
    ///
    m_scanArguments->targetList = ui->targets->listModel->stringList();
    m_scanArguments->currentTargetToEnumerate = 0;
    m_scanArguments->progress = 0;
    ui->progressBar->reset();
    ///
    /// Getting scan arguments....
    ///
    ui->progressBar->setMaximum(ui->targets->listModel->rowCount());
    ///
    /// start Ip subdomain enumeration...
    ///
    startScan();
    //...
    sendStatus("[*] Testing For Ip Subdomains...");
}

void Ip::on_buttonStop_clicked(){
    emit stopScan();
    status->ip->isStopped = true;
}

void Ip::on_buttonConfig_clicked(){
    ConfigDialog *configDialog = new ConfigDialog(this, scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Ip::onClearResults(){
    ///
    /// clear the results...
    ///
    result->ip->subdomainIp->clear();
    ui->labelResultsCount->clear();
    result->ip->subdomainIp->setHorizontalHeaderLabels({"IpAddress", "HostName"});
    ///
    /// clear the progressbar...
    ui->progressBar->clearMask();
    ui->progressBar->reset();
    ui->progressBar->hide();
    ///
    /// hide the filter...
    ///
    ui->buttonFilter->hide();
    ui->lineEditFilter->hide();
    ui->comboBoxFilter->hide();
}

void Ip::onShowFilter(bool show){
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

void Ip::on_comboBoxOption_currentIndexChanged(int index){
    Q_UNUSED(index);
}

void Ip::on_buttonFilter_clicked(){
    QString filterKeyword = ui->lineEditFilter->text();
    result->ip->subdomainIpProxy->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
    result->ip->subdomainIpProxy->setFilterRegExp(filterKeyword);
    ui->tableViewResults->setModel(result->ip->subdomainIpProxy);
}
