#include "Active.h"
#include "ui_Active.h"
//...
#include <QThread>
#include <QDateTime>


/*
 * store all wildcard ip into a database & check if domain check by the
 * active modules contains either of the wildcard ip
 */
Active::Active(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Active),
    m_scanArguments(new active::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Active");
    ///
    /// ...
    ///
    ui->progressBar->hide();
    //...
    ui->buttonStop->setDisabled(true);
    //ui->buttonPause->setDisabled(true);
    //...
    result->active->subdomainIp->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    ui->tableViewResults->setModel(result->active->subdomainIpProxy);
    ///
    /// equsubdomainIpy seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    ///
    /// ...
    ///
    ui->frameCustom->hide();
    //...
    ui->lineEditFilter->hide();
    ui->buttonFilter->hide();
    ui->comboBoxFilter->hide();
    //...
    ui->lineEditServiceName->setPlaceholderText("e.g SMTP");
    ui->lineEditServicePort->setPlaceholderText("e.g 889");
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ///
    /// ...
    ///
    connectActions();
    result->active->subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->active->subdomainIpProxy->setRecursiveFilteringEnabled(true);
}
Active::~Active(){
    delete m_scanArguments;
    delete ui;
}

void Active::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Active::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Active::on_buttonStart_clicked(){
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
    if(ui->comboBoxOption->currentIndex() == ACTIVE::DNS){
        m_scanArguments->checkActiveService = false;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::HTTP){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 80;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::HTTPS){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 443;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::FTP){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 21;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::SMTP){
        m_scanArguments->checkActiveService = true;
        m_scanArguments->service = 587;
    }
    ui->progressBar->setMaximum(ui->targets->listModel->rowCount());
    ///
    /// start active subdomain enumeration...
    ///
    startScan();
    //...
    sendStatus("[*] Testing For Active Subdomains...");
}

void Active::on_buttonStop_clicked(){
    emit stopScan();
    status->active->isStopped = true;
}

void Active::on_comboBoxOption_currentIndexChanged(int index){
    if(index == ACTIVE::DNS){
        ui->label_details->setText("Resolves the target hostname To it's IpAddress");
    }
    if(index == ACTIVE::HTTP){
        ui->label_details->setText("Resolves the target, if Resolved, Then tests for connection To port 80");
    }
    if(index == ACTIVE::HTTPS){
        ui->label_details->setText("Resolves the target, if Resolved Then tests for connection To port 443");
    }
    if(index == ACTIVE::FTP){
        ui->label_details->setText("Resolves the target, if Resolved Then tests for connection To port 20");
    }
    if(index == ACTIVE::SMTP){
        ui->label_details->setText("Resolves the target, if Resolved Then tests for connection To port 587");
    }
}

void Active::on_buttonConfig_clicked(){
    ConfigDialog *configDialog = new ConfigDialog(this, scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Active::onClearResults(){
    ///
    /// clear the results...
    ///
    result->active->subdomainIp->clear();
    ui->labelResultsCount->clear();
    result->active->subdomainIp->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
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

void Active::onShowFilter(bool show){
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

void Active::on_checkBoxCustomActive_clicked(bool checked){
    if(checked){
        ui->frameDefault->hide();
        ui->frameCustom->show();
    }else{
        ui->frameCustom->hide();
        ui->frameDefault->show();
    }
}

void Active::on_buttonFilter_clicked(){
    QString filterKeyword = ui->lineEditFilter->text();
    result->active->subdomainIpProxy->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
    result->active->subdomainIpProxy->setFilterRegExp(filterKeyword);
    ui->tableViewResults->setModel(result->active->subdomainIpProxy);
}
