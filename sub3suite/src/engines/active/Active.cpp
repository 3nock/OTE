#include "Active.h"
#include "ui_Active.h"

#include <QThread>
#include <QDateTime>
#include "src/dialogs/ActiveConfigDialog.h"


Active::Active(QWidget *parent, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, project, status),
    ui(new Ui::Active),
    m_scanConfig(new active::ScanConfig),
    m_scanArgs(new active::ScanArgs),
    m_targetListModel(new QStringListModel),
    m_resultModel(new QStandardItemModel),
    m_resultProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    /* targets */
    ui->targets->setListName("Targets");
    ui->targets->setListModel(m_targetListModel);

    /* result model */
    m_resultModel->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    m_resultProxyModel->setSourceModel(m_resultModel);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->tableViewResults->setModel(m_resultProxyModel);

    /* hiding widgets */
    ui->frameCustom->hide();
    ui->progressBar->hide();
    ui->buttonStop->setDisabled(true);

    /* equsubdomainIpy seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));

    /* placeholdertext */
    ui->lineEditServiceName->setPlaceholderText("e.g SMTP");
    ui->lineEditServicePort->setPlaceholderText("e.g 889");
    ui->lineEditFilter->setPlaceholderText("Enter filter...");

    /* ... */
    this->m_initActions();

    /* syntax higlighting... */
    m_notesSyntaxHighlighter = new NotesSyntaxHighlighter(ui->plainTextEditNotes->document());

    /* config... */
    m_scanArgs->config = m_scanConfig;
}
Active::~Active(){
    delete m_scanConfig;
    delete m_scanArgs;
    delete m_targetListModel;
    delete m_resultModel;
    delete m_resultProxyModel;
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
    /* checking if subdomainIp requirements are satisfied before scan if not prompt error
       then exit function... */
    if(!(m_targetListModel->rowCount() > 0)){
        QMessageBox::warning(this, "Error!", "Please Enter the subdomains Wordlist for Enumeration!");
        return;
    }

    /* disabling and Enabling widgets... */
    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();

    /* Resetting the scan arguments values... */
    m_scanArgs->targetList = m_targetListModel->stringList();
    m_scanArgs->currentTargetToEnumerate = 0;
    m_scanArgs->progress = 0;
    ui->progressBar->reset();

    /* Getting scan arguments.... */
    if(ui->comboBoxOption->currentIndex() == ACTIVE::DNS){
        m_scanArgs->checkActiveService = false;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::HTTP){
        m_scanArgs->checkActiveService = true;
        m_scanArgs->service = 80;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::HTTPS){
        m_scanArgs->checkActiveService = true;
        m_scanArgs->service = 443;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::FTP){
        m_scanArgs->checkActiveService = true;
        m_scanArgs->service = 21;
    }
    if(ui->comboBoxOption->currentIndex() == ACTIVE::SMTP){
        m_scanArgs->checkActiveService = true;
        m_scanArgs->service = 587;
    }
    ui->progressBar->setMaximum(m_targetListModel->rowCount());

    /* start active subdomain enumeration... */
    this->m_startScan();
    sendStatus("[*] Testing For Active Subdomains...");
}

void Active::on_buttonStop_clicked(){
    emit stopScanThread();
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
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
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
