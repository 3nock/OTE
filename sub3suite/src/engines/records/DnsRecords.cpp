#include "DnsRecords.h"
#include "ui_DnsRecords.h"

#include <QDateTime>
#include <QClipboard>
#include "src/dialogs/ActiveConfigDialog.h"


DnsRecords::DnsRecords(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::DnsRecords),
    m_scanConfig(new records::ScanConfig),
    m_scanArgs(new records::ScanArgs),
    m_targetListModel(new QStringListModel),
    m_srvWordlitsModel(new QStringListModel)
{
    ui->setupUi(this);

    /* init... */
    ui->targets->setListName("Targets");
    ui->srvWordlist->setListName("SRV");
    ui->targets->setListModel(m_targetListModel);
    ui->srvWordlist->setListModel(m_srvWordlitsModel);

    /* widgets... */
    ui->buttonStop->setDisabled(true);
    ui->srvWordlist->hide();
    ui->progressBar->hide();
    ui->progressBarSRV->hide();

    /* placeholder texts */
    ui->lineEditFilter->setPlaceholderText("Enter filter...");

    /* results models */
    result->records->dns->setHorizontalHeaderLabels({"Name", "Target", "Port"});
    ui->treeViewResults->setModel(result->records->dns);
    ui->tableViewSRV->setModel(result->records->srv);

    /* ... */
    result->records->dnsProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->records->dnsProxy->setRecursiveFilteringEnabled(true);
    result->records->srvProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->records->srvProxy->setRecursiveFilteringEnabled(true);

    /* registering meta-objects */
    qRegisterMetaType<records::Results>("records::Results");

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));

    /* initiate all actions for the context menus */
    this->m_initActions();
    this->m_loadSrvWordlist();

    /* syntax higlighting... */
    m_notesSyntaxHighlighter = new NotesSyntaxHighlighter(ui->plainTextEditNotes->document());

    /* ... */
    m_scanArgs->config = m_scanConfig;
}
DnsRecords::~DnsRecords(){
    delete m_scanArgs;
    delete m_targetListModel;
    delete m_srvWordlitsModel;
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
    /*
     checking if subdomainIp requirements are satisfied before scan if not prompt error
     then exit function...
    */
    if(m_targetListModel->rowCount() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS) && (!ui->checkBoxA->isChecked() && !ui->checkBoxAAAA->isChecked() && !ui->checkBoxMX->isChecked() && !ui->checkBoxNS->isChecked() && !ui->checkBoxTXT->isChecked() && !ui->checkBoxCNAME->isChecked())){
        QMessageBox::warning(this, "Error!", "Please Choose DNS Record To Enumerate!");
        return;
    }
    if((ui->comboBoxOption->currentIndex() == OPTION::SRV)&& (m_srvWordlitsModel->rowCount() < 1)){
        QMessageBox::warning(this, "Error!", "Please Enter SRV Wordlist For Enumeration!");
        return;
    }

    /* disabling & Enabling widgets... */
    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);

    /* Resetting the scan arguments values... */
    m_scanArgs->targetList = m_targetListModel->stringList();
    m_scanArgs->srvWordlist = m_srvWordlitsModel->stringList();
    m_scanArgs->currentSrvToEnumerate = 0;
    m_scanArgs->currentTargetToEnumerate = 0;

    /* getting the arguments for Dns Records Scan... */
    if(ui->comboBoxOption->currentIndex() == OPTION::ALLRECORDS)
    {
        ui->progressBar->show();
        ui->progressBar->reset();
        m_scanArgs->progress = 0;

        m_scanArgs->RecordType_srv = false;
        m_scanArgs->RecordType_a = ui->checkBoxA->isChecked();
        m_scanArgs->RecordType_aaaa = ui->checkBoxAAAA->isChecked();
        m_scanArgs->RecordType_mx = ui->checkBoxMX->isChecked();
        m_scanArgs->RecordType_ns = ui->checkBoxNS->isChecked();
        m_scanArgs->RecordType_txt = ui->checkBoxTXT->isChecked();
        m_scanArgs->RecordType_cname = ui->checkBoxCNAME->isChecked();

        ui->progressBar->setMaximum(m_targetListModel->rowCount());
    }

    /* getting arguments for SRV DNS Records Scan... */
    if(ui->comboBoxOption->currentIndex() == OPTION::SRV)
    {
        ui->progressBarSRV->show();
        ui->progressBarSRV->reset();
        m_scanArgs->progress = 0;
        m_scanArgs->RecordType_srv = true;
        ui->progressBarSRV->setMaximum(m_targetListModel->rowCount()*m_srvWordlitsModel->rowCount());
    }

    /* start Enumeration... */
    this->m_startScan();
}

void DnsRecords::on_buttonStop_clicked(){
    emit stopScanThread();
    status->dns->isStopped = true;
}

void DnsRecords::m_loadSrvWordlist(){
    QFile file(":/files/res/files/srv.txt");
    ui->srvWordlist->add(file);
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
