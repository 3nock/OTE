#include "Active.h"
#include "ui_Active.h"

Active::Active(QWidget *parent) : QDialog(parent), ui(new Ui::Active),
    model(new QStandardItemModel),
    scanArguments(new ScanArguments_Brute)
{
    ui->setupUi(this);
    //...
    ui->lineEdit->setPlaceholderText(("Enter a new item..."));
    ui->pushButton_stop->setDisabled(true);
    ui->splitter_3->setSizes(QList<int>()<<150<<1);

    QStringList headerLabels = {"Subdomain Name:", "IpAddress"};
    model->setHorizontalHeaderLabels(headerLabels);
    ui->tableView_results->setModel(model);

    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(188, 188, 141));
    p.setColor(QPalette::HighlightedText, QColor(Qt::black));
    ui->tableView_results->setPalette(p);

    currentPath = QDir::currentPath();
}
Active::~Active(){
    delete ui;
    delete model;
}

void Active::on_pushButton_start_clicked(){
    if(!(ui->listWidget_wordlist->count() > 0)){
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter the subdomains Wordlist for Enumeration!");
        return;
    }
    ui->pushButton_start->setDisabled(true);
    ui->pushButton_stop->setEnabled(true);
    ui->progressBar->show();
    //...
    ui->progressBar->setMaximum(ui->listWidget_wordlist->count());
    //...
    scanArguments->wordlist = ui->listWidget_wordlist;
    //...
    startEnumeration();
    //...
    sendStatus("[*] Testing For Active Subdomains...");
    logs("[START] Testing For Active Subdomains...");
}

void Active::on_pushButton_stop_clicked(){
    emit stop();
}

void Active::startEnumeration(){
    int maxThreads = scanArguments->maxThreads;
    int wordlistCount = ui->listWidget_wordlist->count();
    if(maxThreads > wordlistCount){
        maxThreads = wordlistCount;
    }
    activeThreads = maxThreads;
    scanArguments->currentWordlistToEnumerate = 0;
    for(int i = 0; i != maxThreads; i++){
        //...
        ActiveEnumerator *Enumerator = new ActiveEnumerator(scanArguments);
        QThread *cThread = new QThread;
        Enumerator->Enumerate(cThread);
        Enumerator->moveToThread(cThread);
        //...
        connect(Enumerator, SIGNAL(resolvedSubdomain(QString, QString)), this, SLOT(resolvedSubdomain(QString, Qstring)));
        connect(Enumerator, SIGNAL(progressBarValue(int)), ui->progressBar, SLOT(setValue(int)));
        connect(Enumerator, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onThreadEnd()));
        connect(cThread, SIGNAL(finished()), Enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stop()), Enumerator, SLOT(onStop()));
        //...
        cThread->start();
    }
}

void Active::resolvedSubdomain(QString subdomain, QString ipAddress){
    model->setItem(subdomainCount, 0, new QStandardItem(subdomain));
    model->setItem(subdomainCount, 1, new QStandardItem(ipAddress));
    subdomainCount++;
    ui->label_subdomainsCount->setNum(subdomainCount);
}

void Active::onThreadEnd(){
    endedThreads++;
    if(endedThreads == activeThreads){
        endedThreads = 0;
        ui->pushButton_start->setEnabled(true);
        ui->pushButton_stop->setDisabled(true);
        //...
        sendStatus("[*] Enumeration by activeSubdomains Complete!");
        logs("[END] Active Subdomains Enumeration Complete!\n");
    }
}

void Active::on_toolButton_config_clicked(){
    BruteConfigDialog *scanConfig = new BruteConfigDialog(this, scanArguments);
    scanConfig->setAttribute( Qt::WA_DeleteOnClose, true );
    scanConfig->show();
}

void Active::on_pushButton_load_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, INFO_LOADFILE, CURRENT_PATH);
    if(!filename.isEmpty()){
        QFile file(filename);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&file);
            while (!in.atEnd()){
                ui->listWidget_wordlist->addItem(in.readLine());
                wordlistCount++;
            }
            ui->label_wordlistCount->setNum(wordlistCount);
            file.close();
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Failed To Open the File!");
        }
    }
}

void Active::on_pushButton_add_clicked(){
    if(ui->lineEdit->text() != EMPTY){
        ui->listWidget_wordlist->addItem(ui->lineEdit->text());
        ui->lineEdit->clear();
        wordlistCount++;
        ui->label_wordlistCount->setNum(wordlistCount);
    }
}
void Active::on_lineEdit_returnPressed(){
    on_pushButton_add_clicked();
}

void Active::on_pushButton_clearWordlist_clicked(){
    ui->listWidget_wordlist->clear();
    ui->label_wordlistCount->clear();
    wordlistCount = 0;
    //...
    model->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
}

void Active::on_pushButton_remove_clicked(){
    int selectionCount = ui->listWidget_wordlist->selectedItems().count();
    if(selectionCount){
        qDeleteAll(ui->listWidget_wordlist->selectedItems());
        wordlistCount = wordlistCount-selectionCount;
    }
    ui->label_wordlistCount->setNum(wordlistCount);
}

void Active::on_pushButton_clearResults_clicked(){
    // if the current tab is subdomains clear subdomains if logs clear logs...
    if(ui->tabWidget_results->currentIndex() == 0){
        model->clear();
        ui->label_subdomainsCount->clear();
        subdomainCount = 0;
    }else{
        ui->listWidget_logs->clear();
    }
}

void Active::logs(QString log){
    sendLog(log);
    ui->listWidget_logs->addItem(log);
    if(log.startsWith("[ERROR]")){
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]")){
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}

void Active::on_pushButton_action_clicked(){
    // getting the position of the action button to place the context menu...
    QPoint pos = ui->pushButton_action->mapToGlobal(QPoint(0,0));
    // showing the context menu right by the side of the action button...
    showContextMenu_actionButton(QPoint(pos.x()+76, pos.y()));
}

void Active::on_tableView_results_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    // check if user right clicked on items else dont show the context menu...
    if(!ui->tableView_results->selectionModel()->isSelected(ui->tableView_results->currentIndex())){
        return;
    }
    showContextMenu_rightClick();
}

void Active::on_pushButton_get_clicked(){

}

/********************************* action Context Menu ************************************/
/*
    Re-create this functionallity on the constructor for Efficiency...
*/
void Active::showContextMenu_actionButton(const QPoint &position){
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    //...
    QAction actionSendToSave("Send To Save", this);
    QAction actionSendToMultiLevel("Send To Multi-level Scan");
    QAction actionSendToDnsRecords("Send To DnsRecords");
    //...
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave()));
    connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords()));
    connect(&actionSendToMultiLevel, SIGNAL(triggered()), this, SLOT(actionSendToMultiLevel()));
    //...
    menu->addSeparator();
    menu->addAction(&actionSendToDnsRecords);
    menu->addAction(&actionSendToSave);
    menu->addAction(&actionSendToMultiLevel);
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(position);
    menu->exec();
}

/********************************* right-click Context Menu ************************************/
void Active::showContextMenu_rightClick(){
    //...
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    QAction actionSendToSave("Send Selected To Save", this);
    QAction actionSendToDnsRecords("Send Selected To DnsRecords");
    QAction actionOpenInBrowser("Open Selected in Browser");
    //...
    connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser()));
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave()));
    connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(cursorSendToDnsRecords()));
    //...
    menu->addAction(&actionOpenInBrowser);
    menu->addSeparator();
    menu->addAction(&actionSendToDnsRecords);
    menu->addAction(&actionSendToSave);
    //...
    QPoint globalCursorPos = QCursor::pos();
    QRect mouseScreenGeometry = qApp->desktop()->screen(qApp->desktop()->screenNumber(globalCursorPos))->geometry();
    QPoint localCursorPosition = globalCursorPos - mouseScreenGeometry.topLeft();
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(localCursorPosition);
    menu->exec();
}

void Active::actionSendToSave(){
    /*
    int resultsCount = ui->listWidget_subdomains->count();
    for(int i = 0; i != resultsCount; ++i){
        emit sendResultsToSave(ui->listWidget_subdomains->item(i)->text());
    }
    logs("[*] Sent "+QString::number(resultsCount)+" activeSubdomains Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();
    */
}

void Active::actionSendToDnsRecords(){

}

void Active::actionSendToMultiLevel(){

}

void Active::cursorSendToSave(){
    /*foreach(QListWidgetItem * item, ui->listWidget_subdomains->selectedItems()){
        emit sendResultsToSave(item->text());
    }
    logs("[*] Sent "+QString::number(ui->listWidget_subdomains->count())+" activeSubdomains Enumerated Subdomains To Save Tab...");
    emit changeTabToSave();*/
}

void Active::cursorOpenInBrowser(){
    foreach(const QModelIndex &index, ui->tableView_results->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}

void Active::cursorSendToDnsRecords(){

}

