#include "Brute.h"
#include "ui_Brute.h"

#include <QDateTime>
#include "src/utils/Definitions.h"
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/dialogs/wordlist/WordlistDialog.h"


Brute::Brute(QWidget *parent, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, project, status),
    ui(new Ui::Brute),
    m_scanConfig(new brute::ScanConfig),
    m_scanArgs(new brute::ScanArgs),
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

    /* ... */
    m_scanArgs->config = m_scanConfig;
}
Brute::~Brute(){
    delete m_scanConfig;
    delete m_scanArgs;
    delete m_targetListModel;
    delete m_wordlistModel;
    delete m_resultModelSubdomain;
    delete m_resultModelTld;
    delete m_resultProxyModel;
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
    /*
     checking if subdomainIp requirements are satisfied before scan if not prompt error
     then exit function...
    */
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

    /* disabling & Enabling widgets... */
    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);
    ui->progressBar->show();

    /* Resetting the scan arguments values...*/
    m_scanArgs->wordlist = m_wordlistModel->stringList();
    m_scanArgs->currentWordlistToEnumerate = 0;
    m_scanArgs->currentTargetToEnumerate = 0;
    m_scanArgs->targetList.clear();
    ui->progressBar->reset();
    m_scanArgs->progress = 0;

    switch (ui->comboBoxOutput->currentIndex())
    {
    /* Processing targets if user chooses subdomain bruteForcing */
    case brute::OUTPUT::SUBDOMAIN:
        m_scanArgs->subBrute = true;
        m_scanArgs->tldBrute = false;

        if(!ui->checkBoxMultipleTargets->isChecked()){
            m_scanArgs->targetList.append(ui->lineEditTarget->text());

            /* for a single target, progress equals to the total number of wordlist... */
            ui->progressBar->setMaximum(m_wordlistModel->rowCount());
        }

        if(ui->checkBoxMultipleTargets->isChecked()){
            m_scanArgs->targetList = m_targetListModel->stringList();

            /* for multiple targets, progress equals to the total number of wordlist times the total number of targets... */
            ui->progressBar->setMaximum(m_wordlistModel->rowCount()*m_scanArgs->targetList.count());
        }
        break;

    /* Processing targets if user chooses TLD bruteForcing */
    case brute::OUTPUT::TLD:
        m_scanArgs->tldBrute = true;
        m_scanArgs->subBrute = false;

        if(!ui->checkBoxMultipleTargets->isChecked()){
            m_scanArgs->targetList.append(ui->lineEditTarget->text());

            /* for a single target, progress equals to the total number of wordlist... */
            ui->progressBar->setMaximum(m_wordlistModel->rowCount());
        }

        if(ui->checkBoxMultipleTargets->isChecked()){
            m_scanArgs->targetList = m_targetListModel->stringList();

            /* for multiple targets, progress equals to the total number of wordlist times the total number of targets... */
            ui->progressBar->setMaximum(m_wordlistModel->rowCount()*m_scanArgs->targetList.count());
        }
    }

    /* Starting the scan...*/
    this->m_startScan();
}
void Brute::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Brute::on_buttonStop_clicked(){
    emit stopScanThread();
    if(status->brute->isPaused)
    {
        m_scanArgs->targetList.clear();
        status->brute->isPaused = false;
        status->brute->isStopped = false;
        status->brute->isRunning = false;

        /* enabling and disabling widgets */
        ui->buttonStart->setEnabled(true);
        ui->buttonStop->setDisabled(true);
    }
    status->brute->isStopped = true;
}

void Brute::on_buttonConfig_clicked(){
    ActiveConfigDialog *configDialog = new ActiveConfigDialog(this);
    configDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    configDialog->show();
}

void Brute::on_buttonWordlist_clicked(){
    WordListDialog *wordlistDialog = nullptr;

    switch (ui->comboBoxOutput->currentIndex()) {
    case brute::OUTPUT::SUBDOMAIN:
        wordlistDialog = new WordListDialog(this, ENGINE::SUBBRUTE);
        break;
    case brute::OUTPUT::TLD:
        wordlistDialog = new WordListDialog(this, ENGINE::TLDBRUTE);
    }

    wordlistDialog->setAttribute( Qt::WA_DeleteOnClose, true );
    connect(wordlistDialog, &WordListDialog::choosenWordlist, this, &Brute::onChoosenWordlist);
    wordlistDialog->show();
}

void Brute::onChoosenWordlist(QString wordlistFilename){
    QFile file(wordlistFilename);
    ui->wordlist->add(file);
}

void Brute::on_checkBoxMultipleTargets_stateChanged(int newState){
    if(newState == Qt::Checked)
        ui->targets->show();
    else
        ui->targets->hide();
}
