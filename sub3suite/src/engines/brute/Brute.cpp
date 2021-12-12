#include "Brute.h"
#include "ui_Brute.h"
//...
#include <QDateTime>
#include "src/utils/Definitions.h"
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/dialogs/wordlist/WordlistDialog.h"

/*
 * option to resolve both A & AAAA, 3 models, one for A one for AAAA & one for A&AAAA
 *
 * {"www", "online", "webserver", "ns", "ns1", "mail", "smtp", "webmail", "shop", "dev",
            "prod", "test", "vpn", "ftp", "ssh", "secure", "whm", "admin", "webdisk", "mobile",
            "remote", "server", "cpanel", "cloud", "autodiscover", "api", "m", "blog"}

 * homoglyphs - in wordlist creation...
 * many sites are created from homoglyphs of existing ones
 * eg example.com ==> exannple.com, exampl3.com, exqmple.com, exampls.com, exampke.com
 *
 * use a hash-table instead of QSet to to save already present data
 *
 * have a tab to put subdomainIp info/issue definitions,eg asn numbers and their respective
 * owner, ip's and their respective owners...
 *
 *
 **
 fix filter from what you have learned, it shud not be asocciated with scope...

 check if connected to the internet...
 check if host is active...
 pop an alert to user if wants cont...
 Use QNetworkConfigurationManager::isOnline().

*/
/*
 create a scrapper tab: use c++...
*/


/*

  add an advanced option that resolves an item on wordlist with subdomainIp nameservers..
  first obtains the server from the domain name, then uses those nameservers as default nameservers
  for the scan...

*/

/*

  add a function to first test if the domain you are testing is an active one
  to reduce un-necessary lookups

*/

/*

 create a notes tab that takes subdomainIp notes for the related scans of the project...
 each new domain enumerated and its x-stics are to be recorded...
 use of wildcard in each phase...
*/

/*
 * have seperate stylesheet for each os, choose appropriate stylesheet during loading time
 * using macro which indicates what os is compiled in for better visuals...
 *
 * must be project base, on launch asks if you want a quick scan or launch a project base...
 *
 *
    "8.8.8.8",        // Google
    "1.1.1.1",        // Cloudflare
    "9.9.9.9",        // Quad9
    "208.67.222.222", // Cisco OpenDNS
    "84.200.69.80",   // DNS.WATCH
    "64.6.64.6",      // Verisign
    "8.26.56.26",     // Comodo Secure DNS
    "64.6.64.6",      // Neustar DNS
    "195.46.39.39",   // SafeDNS
    "185.228.168.9",  // CleanBrowsing
    "76.76.19.19",    // Alternate DNS
    "77.88.8.1",      // Yandex.DNS
    "94.140.14.140",  // AdGuard
    "216.146.35.35",  // Dyn
    "192.71.245.208", // OpenNIC
    "38.132.106.139", // CyberGhost
    "109.69.8.51",    // puntCAT
    "74.82.42.42",    // Hurricane Electric
 */

Brute::Brute(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Brute),
    m_scanArguments(new brute::ScanArguments)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    ui->wordlist->init("Wordlist");
    targets = ui->targets;
    scanConfig->name = tr("ScanConfig-Brute");
    ///
    /// ...
    ///
    ui->lineEditTarget->setPlaceholderText(PLACEHOLDERTEXT_DOMAIN);
    ui->lineEditFilter->setPlaceholderText("Enter filter...");
    ///
    /// ...
    ///
    ui->buttonStop->setDisabled(true);
    ui->targets->hide();
    ui->progressBar->hide();
    //...
    ui->lineEditFilter->hide();
    ui->buttonFilter->hide();
    ui->comboBoxFilter->hide();
    ///
    /// equsubdomainIpy seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    result->brute->subdomainIp->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    ui->tableViewResults->setModel(result->brute->subdomainIpProxy);
    ///
    /// ...
    ///
    this->connectActions();
    result->brute->subdomainIpProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
    result->brute->subdomainIpProxy->setRecursiveFilteringEnabled(true);
    ///
    /// syntax higlighting...
    ///
    m_notesSyntaxHighlighter = new NotesSyntaxHighlighter(ui->plainTextEditNotes->document());
}
Brute::~Brute(){
    delete m_scanArguments;
    //...
    delete ui;
}

void Brute::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Brute::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Brute::on_buttonStart_clicked(){
    ///
    /// checking if subdomainIp requirements are satisfied before scan if not prompt error
    /// then exit function...
    ///
    if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
        QMessageBox::warning(this, "Error!", "Please Enter the Target for Enumeration!");
        return;
    }
    if(ui->checkBoxMultipleTargets->isChecked() && ui->targets->listModel->rowCount() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter the Targets for Enumeration!");
        return;
    }
    if(ui->wordlist->listModel->rowCount() < 1){
        QMessageBox::warning(this, "Error!", "Please Enter the Wordlist for Enumeration!");
        return;
    }
    ///
    /// disabling & Enabling widgets...
    ///
    ui->buttonStart->setDisabled(true);
    //ui->buttonPause->setEnabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();
    ///
    /// Resetting the scan arguments values...
    ///
    m_scanArguments->wordlist = ui->wordlist->listModel->stringList();
    m_scanArguments->currentWordlistToEnumerate = 0;
    m_scanArguments->currentTargetToEnumerate = 0;
    m_scanArguments->targetList.clear();
    ui->progressBar->reset();
    m_scanArguments->progress = 0;

    /****** Processing targets if user chooses subdomain bruteForcing *******/
    if(ui->comboBoxBruteType->currentIndex() == 0)
    {
        m_scanArguments->subBrute = true;
        m_scanArguments->tldBrute = false;

        if(!ui->checkBoxMultipleTargets->isChecked())
        {
            m_scanArguments->targetList.append(TargetNameFilter(ui->lineEditTarget->text(), ENGINE::SUBBRUTE));
            ///
            /// for a single target, progress equals to the total number of wordlist...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listModel->rowCount());
        }
        if(ui->checkBoxMultipleTargets->isChecked())
        {
            m_scanArguments->targetList = ui->targets->listModel->stringList();
            ///
            /// for multiple targets, progress equals to the total number of wordlist times the total number of targets...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listModel->rowCount()*m_scanArguments->targetList.count());
        }
    }

    /****** Processing targets if user chooses TLD bruteForcing *******/
    if(ui->comboBoxBruteType->currentIndex() == 1)
    {
        m_scanArguments->tldBrute = true;
        m_scanArguments->subBrute = false;

        if(!ui->checkBoxMultipleTargets->isChecked())
        {
            m_scanArguments->targetList.append(TargetNameFilter(ui->lineEditTarget->text(), ENGINE::TLDBRUTE));
            ///
            /// for a single target, progress equals to the total number of wordlist...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listModel->rowCount());
        }
        if(ui->checkBoxMultipleTargets->isChecked())
        {
            m_scanArguments->targetList = ui->targets->listModel->stringList();
            ///
            /// for multiple targets, progress equals to the total number of wordlist times the total number of targets...
            ///
            ui->progressBar->setMaximum(ui->wordlist->listModel->rowCount()*m_scanArguments->targetList.count());
        }
    }
    ///
    /// Starting the scan...
    ///
    startScan();
    //...
    sendStatus("[START] Started Subdomain Enumeration!");
}
void Brute::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Brute::on_buttonStop_clicked(){
    emit stopScan();
    if(status->brute->isPaused)
    {
        m_scanArguments->targetList.clear();
        status->brute->isPaused = false;
        status->brute->isStopped = false;
        status->brute->isRunning = false;
        //...
        ui->buttonStart->setEnabled(true);
        //ui->buttonPause->setDisabled(true);
        ui->buttonStop->setDisabled(true);
        //...
        sendStatus("[*] Enumeration Complete!");
    }
    status->brute->isStopped = true;
}

void Brute::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Brute::onClearResults(){
    ///
    /// clear the results...
    ///
    m_subdomainsSet.clear();
    result->brute->subdomainIp->clear();
    ui->labelResultsCount->clear();
    result->brute->subdomainIp->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
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

void Brute::onShowFilter(bool show){
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

void Brute::on_buttonWordlist_clicked(){
    WordListDialog *wordlistDialog = nullptr;
    if(ui->comboBoxBruteType->currentIndex() == 0){
        wordlistDialog = new WordListDialog(this, ENGINE::SUBBRUTE);
    }
    if(ui->comboBoxBruteType->currentIndex() == 1){
        wordlistDialog = new WordListDialog(this, ENGINE::TLDBRUTE);
    }
    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    //...
    connect(wordlistDialog, &WordListDialog::choosenWordlist, this, &Brute::onChoosenWordlist);
    wordlistDialog->show();
}

void Brute::onChoosenWordlist(QString wordlistFilename){
    QFile file(wordlistFilename);
    ui->wordlist->add(file);
}

void Brute::on_checkBoxMultipleTargets_clicked(bool checked){
    if(checked)
        ui->targets->show();
    else
        ui->targets->hide();
}

void Brute::on_buttonFilter_clicked(){
    QString filterKeyword = ui->lineEditFilter->text();
    result->brute->subdomainIpProxy->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());
    result->brute->subdomainIpProxy->setFilterRegExp(filterKeyword);
    ui->tableViewResults->setModel(result->brute->subdomainIpProxy);
}
