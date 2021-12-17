#include "Cert.h"
#include "ui_Cert.h"

#include <QSslKey>
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/utils/Definitions.h"
#include "src/models/CertModel.h"
#include "src/modules/scan/CertScanner.h"


Cert::Cert(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Cert),
    m_scanConfig(new certificate::ScanConfig),
    m_scanArgs(new certificate::ScanArgs),
    m_targetListModel(new QStringListModel)
{
    ui->setupUi(this);

    /* init */
    ui->targets->setListName("Targets");

    /* enabling and disabling widgets */
    ui->progressBar->hide();
    ui->comboBoxOption->hide();
    ui->buttonStop->setDisabled(true);

    /* placeholdertxt */
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);

    /* result models */
    result->cert->subdomain->setHorizontalHeaderLabels({"Subdomains"});
    result->cert->sslCert->setHorizontalHeaderLabels({"Certificate Fingerprints"});
    result->cert->certInfo->setHorizontalHeaderLabels({"Property", "Value"});

    ui->treeViewResults->setModel(result->cert->subdomainProxy);
    result->active->subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->active->subdomainIpProxy->setRecursiveFilteringEnabled(true);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    /* syntax higlighting...*/
    m_notesSyntaxHighlighter = new NotesSyntaxHighlighter(ui->plainTextEditNotes->document());

    /* initiate all actions for the context menus */
    this->m_initActions();

    /* ... */
    m_scanArgs->config = m_scanConfig;
}
Cert::~Cert(){
    delete ui;
    delete m_scanArgs;
    delete m_scanConfig;
    delete m_targetListModel;
}

void Cert::on_buttonStart_clicked(){
    /* check... */
    if(!(m_targetListModel->rowCount() > 0)){
        QMessageBox::warning(this, "Error!", "Please Enter Targets for Enumeration!");
        return;
    }

    /* disabling and Enabling widgets... */
    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();
    ui->progressBar->reset();
    ui->progressBar->setMaximum(m_targetListModel->rowCount());

    /* start scan... */
    this->m_startScan();
}

void Cert::on_buttonStop_clicked(){
    emit stopScanThread();
}

void Cert::onScanThreadEnded(){
    status->cert->activeScanThreads--;

    if(status->cert->activeScanThreads == 0){
        ui->progressBar->setValue(ui->progressBar->maximum());
        ui->buttonStop->setDisabled(true);
        ui->buttonStart->setEnabled(true);
    }
}

void Cert::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Cert::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Cert::on_comboBoxOutput_currentIndexChanged(int index){
    switch (index) {
    case OUTPUT_SUBDOMAIN:
        ui->treeViewResults->setModel(result->cert->subdomainProxy);
        ui->comboBoxOption->hide();
        ui->labelResultsCount->setNum(result->cert->subdomainProxy->rowCount());
        ui->labelInfo->setText("Enumerated Subdomains");
        ui->treeViewResults->setIndentation(0);
        break;
    case OUTPUT_SSLCERT:
        ui->treeViewResults->setModel(result->cert->sslCertProxy);
        ui->comboBoxOption->show();
        ui->labelResultsCount->setNum(result->cert->sslCertProxy->rowCount());
        ui->labelInfo->setText("Enumerated Certificates:");
        ui->treeViewResults->setIndentation(0);
        break;
    case OUTPUT_CERTINFO:
        ui->treeViewResults->setModel(result->cert->certInfo);
        ui->comboBoxOption->hide();
        ui->labelResultsCount->setNum(result->cert->certInfoProxy->rowCount());
        ui->labelInfo->setText("Enumerated Certificates:");
        ui->treeViewResults->setIndentation(20);
        break;
    }
}

void Cert::on_lineEditFilter_textChanged(const QString &filterKeyword){
    switch (ui->comboBoxOutput->currentIndex()) {
    case OUTPUT_SUBDOMAIN:
        result->cert->subdomainProxy->setFilterRegExp(filterKeyword);
        ui->treeViewResults->setModel(result->cert->subdomainProxy);
        ui->labelResultsCount->setNum(result->cert->subdomainProxy->rowCount());
        break;
    case OUTPUT_SSLCERT:
        result->cert->sslCertProxy->setFilterRegExp(filterKeyword);
        ui->treeViewResults->setModel(result->cert->sslCertProxy);
        ui->labelResultsCount->setNum(result->cert->sslCertProxy->rowCount());
        break;
    case OUTPUT_CERTINFO:
        result->cert->certInfoProxy->setFilterRegExp(filterKeyword);
        ui->treeViewResults->setModel(result->cert->certInfoProxy);
        ui->labelResultsCount->setNum(result->cert->certInfoProxy->rowCount());
        break;
    }
}
