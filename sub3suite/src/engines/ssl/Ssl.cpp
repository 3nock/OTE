#include "Ssl.h"
#include "ui_Ssl.h"

#include <QSslKey>
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/utils/Definitions.h"
#include "src/models/SSLModel.h"
#include "src/modules/scan/SSLScanner.h"


Ssl::Ssl(QWidget *parent, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, project, status),
    ui(new Ui::Ssl),
    m_scanConfig(new certificate::ScanConfig),
    m_scanArgs(new certificate::ScanArgs),
    m_targetListModel(new QStringListModel),
    m_resultModelSubdomain(new QStandardItemModel),
    m_resultModelCertId(new QStandardItemModel),
    m_resultModelCertInfo(new QStandardItemModel),
    m_resultProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    /* init */
    ui->targets->setListName("Targets");
    ui->targets->setListModel(m_targetListModel);

    /* result models */
    m_resultModelSubdomain->setHorizontalHeaderLabels({"Subdomains"});
    m_resultModelCertId->setHorizontalHeaderLabels({"Certificate Fingerprints"});
    m_resultModelCertInfo->setHorizontalHeaderLabels({"Property", "Value"});
    m_resultProxyModel->setSourceModel(m_resultModelSubdomain);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->treeViewResults->setModel(m_resultProxyModel);

    /* enabling and disabling widgets */
    ui->progressBar->hide();
    ui->comboBoxOption->hide();
    ui->buttonStop->setDisabled(true);

    /* placeholdertxt */
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);

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
Ssl::~Ssl(){
    delete m_scanArgs;
    delete m_scanConfig;
    delete m_targetListModel;
    delete m_resultModelSubdomain;
    delete m_resultModelCertInfo;
    delete m_resultModelCertId;
    delete m_resultProxyModel;
    delete ui;
}

void Ssl::on_buttonStart_clicked(){
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

void Ssl::on_buttonStop_clicked(){
    emit stopScanThread();
}

void Ssl::onScanThreadEnded(){
    status->cert->activeScanThreads--;

    if(status->cert->activeScanThreads == 0){
        ui->progressBar->setValue(ui->progressBar->maximum());
        ui->buttonStop->setDisabled(true);
        ui->buttonStart->setEnabled(true);
    }
}

void Ssl::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Ssl::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Ssl::on_comboBoxOutput_currentIndexChanged(int index){
    switch (index) {
    case ssl::OUTPUT::SUBDOMAIN:
        m_resultProxyModel->setSourceModel(m_resultModelSubdomain);
        ui->comboBoxOption->hide();
        ui->labelInfo->setText("Enumerated Subdomains");
        ui->treeViewResults->setIndentation(0);
        break;
    case ssl::OUTPUT::CERT_ID:
        m_resultProxyModel->setSourceModel(m_resultModelCertId);
        ui->comboBoxOption->show();
        ui->labelInfo->setText("Enumerated Certificates:");
        ui->treeViewResults->setIndentation(0);
        break;
    case ssl::OUTPUT::CERT_INFO:
        m_resultProxyModel->setSourceModel(m_resultModelCertInfo);
        ui->comboBoxOption->hide();
        ui->labelInfo->setText("Enumerated Certificates:");
        ui->treeViewResults->setIndentation(20);
        break;
    }

    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Ssl::on_lineEditFilter_textChanged(const QString &filterKeyword){
    m_resultProxyModel->setFilterRegExp(filterKeyword);
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}
