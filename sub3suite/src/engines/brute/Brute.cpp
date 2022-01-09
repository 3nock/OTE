#include "Brute.h"
#include "ui_Brute.h"

#include <QRegExp>
#include <QDateTime>
#include <QDesktopWidget>
#include "src/utils/Config.h"
#include "src/utils/Definitions.h"
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/dialogs/wordlist/WordlistDialog.h"


Brute::Brute(QWidget *parent, ProjectDataModel *project) : AbstractEngine(parent, project), ui(new Ui::Brute),
    m_scanConfig(new brute::ScanConfig),
    m_scanArgs(new brute::ScanArgs),
    m_scanStats(new brute::ScanStat),
    m_wordlistModel(new QStringListModel),
    m_targetListModel(new QStringListModel),
    m_resultModelSubdomain(new QStandardItemModel),
    m_resultModelTld(new QStandardItemModel),
    m_resultProxyModel(new QSortFilterProxyModel)
{
    ui->setupUi(this);

    /* wordlist & target models */
    ui->targets->setListName("Targets");
    ui->wordlist->setListName("Wordlist");
    ui->targets->setListModel(m_targetListModel);
    ui->wordlist->setListModel(m_wordlistModel);

    /* results models */
    m_resultModelSubdomain->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    m_resultModelTld->setHorizontalHeaderLabels({"TLD", "IpAddress"});
    /* default is subdomain result model */
    m_resultProxyModel->setSourceModel(m_resultModelSubdomain);
    m_resultProxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    m_resultProxyModel->setRecursiveFilteringEnabled(true);
    m_resultProxyModel->setFilterKeyColumn(0);
    ui->tableViewResults->setModel(m_resultProxyModel);

    /* placeholder texts */
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
    ui->lineEditFilter->setPlaceholderText("Enter filter...");

    /* ...*/
    ui->buttonStop->setDisabled(true);
    ui->targets->hide();
    ui->progressBar->hide();

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));

    /* initiate all actions for the context menus */
    this->m_initActions();

    /* syntax higlighting... */
    m_notesSyntaxHighlighter = new NotesSyntaxHighlighter(ui->plainTextEditNotes->document());

    /* registering meta-objects */
    qRegisterMetaType<scan::Log>("scan::Log");
    qDebug() << "Registering scan::Log for brute";

    /* ... */
    m_scanArgs->config = m_scanConfig;

    /* config values */
    this->m_getConfigValues();
}
Brute::~Brute(){
    delete m_scanConfig;
    delete m_scanArgs;
    delete m_scanStats;
    delete m_targetListModel;
    delete m_wordlistModel;
    delete m_resultModelSubdomain;
    delete m_resultModelTld;
    delete m_resultProxyModel;
    delete ui;
}

void Brute::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void Brute::on_buttonStart_clicked(){
    ///
    /// Start scan...
    ///
    if(status->isNotActive){
        if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
            QMessageBox::warning(this, "Error!", "Please Enter the Target for Enumeration!");
            return;
        }
        if(ui->checkBoxMultipleTargets->isChecked() && m_targetListModel->rowCount() < 1){
            QMessageBox::warning(this, "Error!", "Please Enter the Targets for Enumeration!");
            return;
        }
        if(m_wordlistModel->rowCount() < 1){
            QMessageBox::warning(this, "Error!", "Please Enter the Wordlist for Enumeration!");
            return;
        }

        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText("Pause");

        status->isRunning = true;
        status->isNotActive = false;
        status->isStopped = false;
        status->isPaused = false;

        /* start scan */
        this->m_startScan();

        /* logs */
        m_log("------------------ start ----------------\n");
        qInfo() << "Scan Started";
        return;
    }
    ///
    /// Pause scan...
    ///
    if(status->isRunning){
        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText("Resume");

        status->isPaused = true;
        status->isRunning = false;
        status->isStopped = false;
        status->isNotActive = false;

        /* pause scan */
        emit pauseScanThread();

        /* logs */
        m_log("------------------ Paused ----------------\n");
        qInfo() << "Scan Paused";
        return;
    }
    ///
    /// Resume scan...
    ///
    if(status->isPaused){
        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText("Pause");

        status->isRunning = true;
        status->isPaused = false;
        status->isStopped = false;
        status->isNotActive = false;

        /* resume scan */
        emit resumeScanThread();

        /* logs */
        m_log("------------------ Resumed ----------------\n");
        qInfo() << "Scan Resumed";
    }
}

void Brute::on_buttonStop_clicked(){
    if(status->isPaused)
        emit resumeScanThread();

    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void Brute::on_lineEditFilter_textChanged(const QString &filterKeyword){
    m_resultProxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        m_resultProxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        m_resultProxyModel->setFilterFixedString(filterKeyword);

    ui->tableViewResults->setModel(m_resultProxyModel);
    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Brute::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this, m_scanConfig);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Brute::on_buttonWordlist_clicked(){
    WordListDialog *wordlistDialog = new WordListDialog(this, m_wordlistModel);

    switch (ui->comboBoxOutput->currentIndex()) {
    case brute::OUTPUT::SUBDOMAIN:
        wordlistDialog->initSubdomainWordlist();
        break;
    case brute::OUTPUT::TLD:
        wordlistDialog->initTLDWordlist();
    }

    connect(wordlistDialog, &WordListDialog::wordlistLoaded, this, [=](){ui->wordlist->updateSize();});
    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );

    /* adjust dialog to appear abit up */
    int x = (qApp->desktop()->width()-wordlistDialog->width()) / 2;
    int y = (qApp->desktop()->height()-wordlistDialog->height()) / 2;
    wordlistDialog->move(x, y-100);

    wordlistDialog->show();
}

void Brute::on_checkBoxMultipleTargets_stateChanged(int newState){
    if(newState == Qt::Checked)
        ui->targets->show();
    else
        ui->targets->hide();
}

void Brute::on_comboBoxOutput_currentIndexChanged(int index){
    switch (index) {
    case brute::OUTPUT::SUBDOMAIN:
        m_resultProxyModel->setSourceModel(m_resultModelSubdomain);
        break;
    case brute::OUTPUT::TLD:
        m_resultProxyModel->setSourceModel(m_resultModelTld);
    }

    ui->labelResultsCount->setNum(m_resultProxyModel->rowCount());
}

void Brute::m_log(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

/* get config settings from config file & saving to brute::ScanConfig structure*/
void Brute::m_getConfigValues(){
    m_scanArgs->config->timeout = CONFIG_BRUTE.value("timeout").toInt();
    m_scanArgs->config->threads = CONFIG_BRUTE.value("threads").toInt();
    m_scanArgs->config->levels = CONFIG_BRUTE.value("maxLevel").toInt();
    m_scanArgs->config->checkWildcard = CONFIG_BRUTE.value("wildcard").toBool();
    m_scanArgs->config->multiLevelScan = CONFIG_BRUTE.value("useLevel").toInt();
    m_scanArgs->config->noDuplicates = CONFIG_BRUTE.value("noDuplicates").toBool();
    m_scanArgs->config->autoSaveToProject = CONFIG_BRUTE.value("autosaveToProject").toBool();

    QString record = CONFIG_BRUTE.value("record").toString();
    if(record == "A")
        m_scanArgs->config->recordType = QDnsLookup::A;
    if(record == "AAAA")
        m_scanArgs->config->recordType = QDnsLookup::AAAA;

    int size = CONFIG_BRUTE.beginReadArray("Nameservers");
    for (int i = 0; i < size; ++i) {
        CONFIG_BRUTE.setArrayIndex(i);
        m_scanArgs->config->nameservers.append(CONFIG_BRUTE.value("value").toString());
    }
    CONFIG_BRUTE.endArray();
}
