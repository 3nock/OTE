/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :BRUTEFORCE Engine, obtain subdomains & top level domains(TLD) by resolving bruteforced targets.
*/

#include "Brute.h"
#include "ui_Brute.h"

#include <QRegExp>
#include <QDateTime>
#include <QDesktopWidget>
#include "src/utils/Config.h"
#include "src/utils/Definitions.h"
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/dialogs/WordlistDialog.h"


Brute::Brute(QWidget *parent, ProjectModel *project) : AbstractEngine(parent, project),
    ui(new Ui::Brute),
    m_scanConfig(new brute::ScanConfig),
    m_scanArgs(new brute::ScanArgs),
    m_scanStats(new brute::ScanStat),
    m_wordlistModel(new QStringListModel),
    m_targetListModel(new QStringListModel),
    m_model_subdomain(new QStandardItemModel),
    m_model_tld(new QStandardItemModel)
{
    this->iniUI();

    /* list models */
    ui->targets->setListName(tr("Targets"));
    ui->wordlist->setListName(tr("Wordlist"));
    ui->targets->setListModel(m_targetListModel);
    ui->wordlist->setListModel(m_wordlistModel);

    /* results models */
    m_model_subdomain->setHorizontalHeaderLabels({tr(" Subdomain"), tr(" Ipv4"), tr(" Ipv6")});
    m_model_tld->setHorizontalHeaderLabels({tr(" TLD"), tr(" Ipv4"), tr(" Ipv6")});
    proxyModel->setSourceModel(m_model_subdomain);
    ui->tableViewResults->setModel(proxyModel);

    ui->tableViewResults->horizontalHeader()->resizeSection(0, 200);
    ui->tableViewResults->horizontalHeader()->resizeSection(1, 100);
    ui->tableViewResults->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_scanArgs->config = m_scanConfig;
    this->initConfigValues();
}
Brute::~Brute(){
    delete m_model_tld;
    delete m_model_subdomain;
    delete m_wordlistModel;
    delete m_targetListModel;
    delete m_scanStats;
    delete m_scanArgs;
    delete m_scanConfig;
    delete ui;
}

void Brute::iniUI(){
    ui->setupUi(this);

    /* widget properties */
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);

    /* placeholder texts */
    ui->lineEditTarget->setPlaceholderText(tr(PLACEHOLDERTEXT_DOMAIN));
    ui->lineEditFilter->setPlaceholderText(tr("filter..."));

    /* hiding and disabling some widgets */
    ui->buttonStop->setDisabled(true);
    ui->targets->hide();
    ui->progressBar->hide();

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
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
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Target for Enumeration!"));
            return;
        }
        if(ui->checkBoxMultipleTargets->isChecked() && m_targetListModel->rowCount() < 1){
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Targets for Enumeration!"));
            return;
        }
        if(m_wordlistModel->rowCount() < 1){
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter the Wordlist for Enumeration!"));
            return;
        }

        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText(tr("Pause"));

        status->isRunning = true;
        status->isNotActive = false;
        status->isStopped = false;
        status->isPaused = false;

        /* start scan */
        this->startScan();

        /* logs */
        log("------------------ start ----------------\n");
        qInfo() << "[BRUTE] Scan Started";
        return;
    }
    ///
    /// Pause scan...
    ///
    if(status->isRunning){
        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText(tr("Resume"));

        status->isPaused = true;
        status->isRunning = false;
        status->isStopped = false;
        status->isNotActive = false;

        /* pause scan */
        emit pauseScanThread();

        /* logs */
        log("------------------ Paused ----------------\n");
        qInfo() << "[BRUTE] Scan Paused";
        return;
    }
    ///
    /// Resume scan...
    ///
    if(status->isPaused){
        ui->buttonStop->setEnabled(true);
        ui->buttonStart->setText(tr("Pause"));

        status->isRunning = true;
        status->isPaused = false;
        status->isStopped = false;
        status->isNotActive = false;

        /* resume scan */
        emit resumeScanThread();

        /* logs */
        log("------------------ Resumed ----------------\n");
        qInfo() << "[BRUTE] Scan Resumed";
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
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->tableViewResults->setModel(proxyModel);
    ui->labelResultsCount->setNum(proxyModel->rowCount());
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
    wordlistDialog->move(x, y-120);

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
        proxyModel->setSourceModel(m_model_subdomain);
        break;
    case brute::OUTPUT::TLD:
        proxyModel->setSourceModel(m_model_tld);
    }

    ui->labelResultsCount->setNum(proxyModel->rowCount());
}

void Brute::log(const QString &log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void Brute::initConfigValues(){
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
    if(record == "ANY")
        m_scanArgs->config->recordType = QDnsLookup::ANY;

    int size = CONFIG_BRUTE.beginReadArray("Nameservers");
    for (int i = 0; i < size; ++i) {
        CONFIG_BRUTE.setArrayIndex(i);
        m_scanArgs->config->nameservers.enqueue(CONFIG_BRUTE.value("value").toString());
    }
    CONFIG_BRUTE.endArray();
}
