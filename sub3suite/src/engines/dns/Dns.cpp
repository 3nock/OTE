#include "Dns.h"
#include "ui_Dns.h"

#include <QDateTime>
#include <QClipboard>
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/utils/Definitions.h"


Dns::Dns(QWidget *parent, ProjectDataModel *project) : AbstractEngine(parent, project), ui(new Ui::Dns),
    m_scanConfig(new dns::ScanConfig),
    m_scanArgs(new dns::ScanArgs),
    m_targetListModel(new QStringListModel),
    m_srvWordlitsModel(new QStringListModel),
    m_resultModel(new QStandardItemModel),
    m_resultProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    /* init... */
    ui->targets->setListName("Targets");
    ui->srvWordlist->setListName("SRV");
    ui->targets->setListModel(m_targetListModel);
    ui->srvWordlist->setListModel(m_srvWordlitsModel);

    /* result model */
    m_resultModel->setHorizontalHeaderLabels({"Dns Records"});
    m_resultProxyModel->setSourceModel(m_resultModel);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->treeViewResults->setModel(m_resultProxyModel);

    /* widgets... */
    ui->buttonStop->setDisabled(true);
    ui->srvWordlist->hide();
    ui->progressBar->hide();

    /* placeholder texts */
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);

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
Dns::~Dns(){
    delete m_scanArgs;
    delete m_targetListModel;
    delete m_srvWordlitsModel;
    delete m_resultModel;
    delete m_resultProxyModel;
    delete ui;
}

void Dns::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Dns::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Dns::on_buttonStart_clicked(){
    /*
     checking if subdomainIp requirements are satisfied before scan if not prompt error
     then exit function...
    */
    if(m_targetListModel->rowCount() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter Target Subdomains For Enumeration!");
        return;
    }
    if((!ui->checkBoxSRV->isChecked()) && (!ui->checkBoxA->isChecked() && !ui->checkBoxAAAA->isChecked() && !ui->checkBoxMX->isChecked() && !ui->checkBoxNS->isChecked() && !ui->checkBoxTXT->isChecked() && !ui->checkBoxCNAME->isChecked())){
        QMessageBox::warning(this, "Error!", "Please Choose DNS Record To Enumerate!");
        return;
    }
    if((ui->checkBoxSRV->isChecked())&& (m_srvWordlitsModel->rowCount() < 1)){
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
    ui->progressBar->show();
    ui->progressBar->reset();
    m_scanArgs->progress = 0;

    m_scanArgs->RecordType_srv = false; // for now
    m_scanArgs->RecordType_a = ui->checkBoxA->isChecked();
    m_scanArgs->RecordType_aaaa = ui->checkBoxAAAA->isChecked();
    m_scanArgs->RecordType_mx = ui->checkBoxMX->isChecked();
    m_scanArgs->RecordType_ns = ui->checkBoxNS->isChecked();
    m_scanArgs->RecordType_txt = ui->checkBoxTXT->isChecked();
    m_scanArgs->RecordType_cname = ui->checkBoxCNAME->isChecked();

    ui->progressBar->setMaximum(m_targetListModel->rowCount());

    /* start Enumeration... */
    this->m_startScan();
}

void Dns::on_buttonStop_clicked(){
    emit stopScanThread();
    status->isStopped = true;
}

void Dns::m_loadSrvWordlist(){
    QFile file(":/files/res/files/srv.txt");
    ui->srvWordlist->add(file);
}

void Dns::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Dns::on_checkBoxSRV_clicked(bool checked){
    if(checked)
        ui->srvWordlist->show();
    else
        ui->srvWordlist->hide();
}
