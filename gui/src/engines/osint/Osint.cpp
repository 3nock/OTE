#include "Osint.h"
#include "ui_Osint.h"

/******************************* Constructor & Destructor ********************************/

Osint::Osint(QWidget *parent, ResultsModel *resultsModel) : BaseClass(parent, resultsModel), ui(new Ui::Osint),
    m_resultsModel(resultsModel),
    //...
    scanArguments(new osint::ScanArguments),
    scanResults(new osint::ScanResults)
{
    ui->setupUi(this);
    ///
    /// setting up targets widgets to the base class...
    ///
    widgets->listWidget_targets = ui->listWidget_targets;
    widgets->label_targetsCount = ui->label_targetsCount;
    widgets->lineEdit_targetInput = ui->lineEdit_multipleTargets;
    widgets->listWidget_logs = ui->listWidget_logs;
    ///
    /// other initializations...
    ///
    currentPath = QDir::currentPath();
    ui->lineEdit_domain->setPlaceholderText("eg. example.com");
    ui->lineEdit_newProfile->setPlaceholderText("Enter New Profile's Name...");
    ui->lineEdit_multipleTargets->setPlaceholderText("Enter a new Target...");
    //...
    m_resultsModel->osint->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
    ui->tableView_results->setModel(m_resultsModel->osint);
    //...
    ui->pushButton_pause->setDisabled(true);
    ui->pushButton_stop->setDisabled(true);
    //...
    ui->progressBar->hide();
    ui->frame_targets->hide();
    //...
    ui->splitter->setSizes(QList<int>()<<270<<180);
    //...
    setupOsintProfiles();
}
Osint::~Osint(){
    delete scanArguments;
    delete scanResults;
    delete ui;
}

/************************************* Start Enumeration *********************************/
void Osint::on_pushButton_start_clicked(){
    if(ui->lineEdit_domain->text() != EMPTY){
        getUserOptions(&scanArguments->choosenOptions);
        if(scanArguments->choosenOptions.count() == 0){
            QMessageBox::warning(this, TITLE_ERROR, "Please Choose Osint Engine For subdomain Enumerations!");
            return;
        }
        if(ui->lineEdit_domain->text().isEmpty()){
            QMessageBox::warning(this, TITLE_ERROR, "Please Target Domain For Enumerations!");
            return;
        }
        //...
        ui->pushButton_start->setDisabled(true);
        ui->pushButton_stop->setEnabled(true);
        m_targetDomain = TargetNameFilter(ui->lineEdit_domain->text(), ENGINE::OSINT);
        ///
        /// converting the QString domainName to char* for compatibility with the PyObject methods...
        ///
        char *targetDomain = new char[m_targetDomain.length() + 1];
        strcpy(targetDomain, m_targetDomain.toStdString().c_str());
        //...
        scanArguments->targetDomain = targetDomain;
        scanResults->label_subdomainsCount = ui->label_resultsCount;
        scanResults->resultsCount = &subdomainsCount;
        scanResults->results_model = m_resultsModel->osint;
        //...
        emit sendStatus("[*] Enumerating "+m_targetDomain+" Subdomains with Osint...");
        logs("[START] Enumerating "+m_targetDomain+" Subdomains with Osint...");
        //...
        Enumerator *enumerator = new Enumerator(scanArguments, scanResults);
        QThread *cThread = new QThread(this);
        enumerator->Enumerate(cThread);
        enumerator->moveToThread(cThread);
        //...
        connect(enumerator, SIGNAL(scanLogs(QString)), this, SLOT(logs(QString)));
        connect(enumerator, SIGNAL(enumerationComplete()), this, SLOT(onEnumerationComplete()));
        connect(this, SIGNAL(stopScan()), enumerator, SLOT(onStopEnumeration()));
        connect(cThread, SIGNAL(finished()), enumerator, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        //...
        cThread->start();
    }else{
        QMessageBox::warning(this, TITLE_ERROR, "Please Enter Target Domain Name to Enumerate Subdomains!");
    }
}
void Osint::on_lineEdit_domain_returnPressed(){
    on_pushButton_start_clicked();
}

/****************************************** stop ****************************************/
void Osint::on_pushButton_stop_clicked(){
    emit stopScan();
    sendStatus("[*] Stopping...");
}

/****************************** Enumeration Results  ************************************/
void Osint::onEnumerationComplete(){
    //...
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_stop->setDisabled(true);
    //...
    emit sendStatus("[END] Enumeration Complete!");
    logs("[END] Enumeration Complete!\n");
}

void Osint::on_pushButton_clear_clicked(){
    // clear subdomains...
    if(ui->tabWidget->currentIndex() == 0){
        m_resultsModel->osint->clear();
        ui->label_resultsCount->clear();
        subdomainsCount = 0;
        //...
        m_resultsModel->osint->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
    }
    // clear logs...
    else{
        ui->listWidget_logs->clear();
    }
}

/************************************ Scan Config (Dialogs) **************************************/
void Osint::on_toolButton_keys_clicked(){
    ApiKeysDialog *apiKeysDialog = new ApiKeysDialog(this);
    apiKeysDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeysDialog->show();
}
void Osint::on_toolButton_config_clicked(){
    /*
    OsintConfigDialog *scanConfig = new OsintConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
    */
}

/*****************************************************************************************
                            Multiple Targets
*****************************************************************************************/
void Osint::on_pushButton_removeTargets_clicked(){
    removeTargets();
}

void Osint::on_pushButton_clearTargets_clicked(){
    clearTargets();
}

void Osint::on_pushButton_loadTargets_clicked(){
    loadTargetsFromFile();
}

void Osint::on_pushButton_addTargets_clicked(){
    addTargets();
}

void Osint::on_lineEdit_multipleTargets_returnPressed(){
    addTargets();
}

/******************************************************************
                     OSINT ENGINES OPTIONS
*******************************************************************/
void Osint::getUserOptions(QStringList *choosenEngines){
    if(ui->checkBox_engine_threatminer->isChecked()){
        choosenEngines->append(ENGINE_THREATMINER);
    }
    if(ui->checkBox_engine_shodan->isChecked()){
        choosenEngines->append(ENGINE_SHODAN);
    }
    if(ui->checkBox_engine_otx->isChecked()){
        choosenEngines->append(ENGINE_OTX);
    }
    if(ui->checkBox_engine_netcraft->isChecked()){
        choosenEngines->append(ENGINE_NETCRAFT);
    }
    if(ui->checkBox_engine_censys->isChecked()){
        choosenEngines->append(ENGINE_CENSYS);
    }
    if(ui->checkBox_engine_github->isChecked()){
        choosenEngines->append(ENGINE_GITHUB);
    }
    if(ui->checkBox_engine_certspotter->isChecked()){
        choosenEngines->append(ENGINE_CERTSPOTTER);
    }
    if(ui->checkBox_engine_dogpile->isChecked()){
        choosenEngines->append(ENGINE_DOGPILE);
    }
    if(ui->checkBox_engine_duckduckgo->isChecked()){
        choosenEngines->append(ENGINE_DUCKDUCKGO);
    }
    if(ui->checkBox_engine_exalead->isChecked()){
        choosenEngines->append(ENGINE_EXALEAD);
    }
    if(ui->checkBox_engine_huntersearch->isChecked()){
        choosenEngines->append(ENGINE_HUNTERSEARCH);
    }
    if(ui->checkBox_engine_intelx->isChecked()){
        choosenEngines->append(ENGINE_INTELX);
    }
    if(ui->checkBox_engine_securitytrails->isChecked()){
        choosenEngines->append(ENGINE_SECURITYTRAILS);
    }
    if(ui->checkBox_engine_suip->isChecked()){
        choosenEngines->append(ENGINE_SUIP);
    }
    if(ui->checkBox_engine_trello->isChecked()){
        choosenEngines->append(ENGINE_TRELLO);
    }
    if(ui->checkBox_engine_san->isChecked()){
        choosenEngines->append(ENGINE_SAN);
    }
    if(ui->checkBox_engine_cloudflare->isChecked()){
        choosenEngines->append(ENGINE_CLOUDFLARE);
    }
    if(ui->checkBox_engine_threatcrowd->isChecked()){
        choosenEngines->append(ENGINE_THREATCROWD);
    }
    if(ui->checkBox_engine_dnsbufferoverrun->isChecked()){
        choosenEngines->append(ENGINE_DNSBUFFEROVERRUN);
    }
    if(ui->checkBox_engine_hackertarget->isChecked()){
        choosenEngines->append(ENGINE_HACKERTARGET);
    }
    if(ui->checkBox_engine_pkey->isChecked()){
        choosenEngines->append(ENGINE_PKEY);
    }
    if(ui->checkBox_engine_waybackmachine->isChecked()){
        choosenEngines->append(ENGINE_WAYBACKMACHINE);
    }
    if(ui->checkBox_engine_ask->isChecked()){
        choosenEngines->append(ENGINE_ASK);
    }
    if(ui->checkBox_engine_baidu->isChecked()){
        choosenEngines->append(ENGINE_BAIDU);
    }
    if(ui->checkBox_engine_bing->isChecked()){
        choosenEngines->append(ENGINE_BING);
    }
    if(ui->checkBox_engine_crtsh->isChecked()){
        choosenEngines->append(ENGINE_CRTSH);
    }
    if(ui->checkBox_engine_dnsdumpster->isChecked()){
        choosenEngines->append(ENGINE_DNSDUMPSTER);
    }
    if(ui->checkBox_engine_google->isChecked()){
        choosenEngines->append(ENGINE_GOOGLE);
    }
    if(ui->checkBox_engine_passivedns->isChecked()){
        choosenEngines->append(ENGINE_PASSIVEDNS);
    }
    if(ui->checkBox_engine_virustotal->isChecked()){
        choosenEngines->append(ENGINE_VIRUSTOTAL);
    }
    if(ui->checkBox_engine_yahoo->isChecked()){
        choosenEngines->append(ENGINE_YAHOO);
    }
    if(ui->checkBox_engine_virustotalapi->isChecked()){
        choosenEngines->append(ENGINE_VIRUSTOTALAPI);
    }
    if(ui->checkBox_engine_omnisint->isChecked()){
        choosenEngines->append(ENGINE_OMNISINT);
    }
    if(ui->checkBox_engine_qwant->isChecked()){
        choosenEngines->append(ENGINE_QWANT);
    }
    if(ui->checkBox_engine_rapiddns->isChecked()){
        choosenEngines->append(ENGINE_RAPIDDNS);
    }
    if(ui->checkBox_engine_urlscan->isChecked()){
        choosenEngines->append(ENGINE_URLSCAN);
    }
    if(ui->checkBox_engine_pentesttools->isChecked()){
        choosenEngines->append(ENGINE_PENTESTTOOLS);
    }
    if(ui->checkBox_engine_projectdiscovery->isChecked()){
        choosenEngines->append(ENGINE_PROJECTDISCOVERY);
    }
    if(ui->checkBox_engine_spyse->isChecked()){
        choosenEngines->append(ENGINE_SPYSE);
    }
}

void Osint::setupOsintProfiles(){
    ///
    /// writting profile names on the comboBox...
    ///
    QFile osintProfiles(currentPath+FILE_PROFILES);
    osintProfiles.open(QIODevice::ReadOnly | QIODevice::Text);
    if(osintProfiles.isOpen()){
        QJsonParseError JsonParseError;
        QStringList profile_names = QJsonDocument::fromJson(osintProfiles.readAll(), &JsonParseError).object().keys();
        osintProfiles.close();
        for(int i = 0; i != profile_names.size(); i++){
            ui->comboBox_profiles->addItem(profile_names[i]);
        }
    }else{
        logs("[Error] Failed To Open /config/osint-profiles.json File For Display on Profiles!");
    }
    ///
    /// hiding the profiles Frame...
    ///
    ui->frame_profiles->hide();
}

///
/// show the frameWidget containing the profile's options...
///
void Osint::on_checkBox_useProfiles_clicked(bool checked){
    if(checked){
        ui->frame_profiles->show();
    }else{
        ui->frame_profiles->hide();
    }
}
/****************** loading a profile... ***********************/
void Osint::on_pushButton_loadProfile_clicked(){
    QFile osintProfiles(currentPath+FILE_PROFILES);
    osintProfiles.open(QIODevice::ReadOnly | QIODevice::Text);
    if(osintProfiles.isOpen()){
        QString keys = osintProfiles.readAll();
        osintProfiles.close();
        QJsonDocument apis = QJsonDocument::fromJson(keys.toUtf8());
        QJsonObject keys_object = apis.object()[ui->comboBox_profiles->currentText()].toObject();
        //...
        if(keys_object.value(ENGINE_THREATMINER) == OSINT_TRUE){
            ui->checkBox_engine_threatminer->setChecked(true);
        }else{
            ui->checkBox_engine_threatminer->setChecked(false);
        }
        if(keys_object.value(ENGINE_SHODAN) == OSINT_TRUE){
            ui->checkBox_engine_shodan->setChecked(true);
        }else{
            ui->checkBox_engine_shodan->setChecked(false);
        }
        if(keys_object.value(ENGINE_BING) == OSINT_TRUE){
            ui->checkBox_engine_bing->setChecked(true);
        }else{
            ui->checkBox_engine_bing->setChecked(false);
        }
        if(keys_object.value(ENGINE_GITHUB) == OSINT_TRUE){
            ui->checkBox_engine_github->setChecked(true);
        }else{
            ui->checkBox_engine_github->setChecked(false);
        }
        if(keys_object.value(ENGINE_CENSYS) == OSINT_TRUE){
            ui->checkBox_engine_censys->setChecked(true);
        }else{
            ui->checkBox_engine_censys->setChecked(false);
        }
        if(keys_object.value(ENGINE_SECURITYTRAILS) == OSINT_TRUE){
            ui->checkBox_engine_securitytrails->setChecked(true);
        }else{
            ui->checkBox_engine_securitytrails->setChecked(false);
        }
        if(keys_object.value(ENGINE_CLOUDFLARE) == OSINT_TRUE){
            ui->checkBox_engine_cloudflare->setChecked(true);
        }else{
            ui->checkBox_engine_cloudflare->setChecked(false);
        }
        if(keys_object.value(ENGINE_INTELX) == OSINT_TRUE){
            ui->checkBox_engine_intelx->setChecked(true);
        }else{
            ui->checkBox_engine_intelx->setChecked(false);
        }
        if(keys_object.value(ENGINE_VIRUSTOTAL) == OSINT_TRUE){
            ui->checkBox_engine_virustotal->setChecked(true);
        }else{
            ui->checkBox_engine_virustotal->setChecked(false);
        }
        if(keys_object.value(ENGINE_GOOGLE) == OSINT_TRUE){
            ui->checkBox_engine_google->setChecked(true);
        }else{
            ui->checkBox_engine_google->setChecked(false);
        }
        if(keys_object.value(ENGINE_CERTSPOTTER) == OSINT_TRUE){
            ui->checkBox_engine_certspotter->setChecked(true);
        }else{
            ui->checkBox_engine_certspotter->setChecked(false);
        }
        if(keys_object.value(ENGINE_CRTSH) == OSINT_TRUE){
            ui->checkBox_engine_crtsh->setChecked(true);
        }else{
            ui->checkBox_engine_crtsh->setChecked(false);
        }
        if(keys_object.value(ENGINE_DOGPILE) == OSINT_TRUE){
            ui->checkBox_engine_dogpile->setChecked(true);
        }else{
            ui->checkBox_engine_dogpile->setChecked(false);
        }
        if(keys_object.value(ENGINE_DUCKDUCKGO) == OSINT_TRUE){
            ui->checkBox_engine_duckduckgo->setChecked(true);
        }else{
            ui->checkBox_engine_duckduckgo->setChecked(false);
        }
        if(keys_object.value(ENGINE_EXALEAD) == OSINT_TRUE){
            ui->checkBox_engine_exalead->setChecked(true);
        }else{
            ui->checkBox_engine_exalead->setChecked(false);
        }
        if(keys_object.value(ENGINE_HUNTERSEARCH) == OSINT_TRUE){
            ui->checkBox_engine_huntersearch->setChecked(true);
        }else{
            ui->checkBox_engine_huntersearch->setChecked(false);
        }
        if(keys_object.value(ENGINE_NETCRAFT) == OSINT_TRUE){
            ui->checkBox_engine_netcraft->setChecked(true);
        }else{
            ui->checkBox_engine_netcraft->setChecked(false);
        }
        if(keys_object.value(ENGINE_OTX) == OSINT_TRUE){
            ui->checkBox_engine_otx->setChecked(true);
        }else{
            ui->checkBox_engine_otx->setChecked(false);
        }
        if(keys_object.value(ENGINE_SUIP) == OSINT_TRUE){
            ui->checkBox_engine_suip->setChecked(true);
        }else{
            ui->checkBox_engine_suip->setChecked(false);
        }
        if(keys_object.value(ENGINE_TRELLO) == OSINT_TRUE){
            ui->checkBox_engine_trello->setChecked(true);
        }else{
            ui->checkBox_engine_trello->setChecked(false);
        }
        if(keys_object.value(ENGINE_SAN) == OSINT_TRUE){
            ui->checkBox_engine_san->setChecked(true);
        }else{
            ui->checkBox_engine_san->setChecked(false);
        }
        if(keys_object.value(ENGINE_THREATCROWD) == OSINT_TRUE){
            ui->checkBox_engine_threatcrowd->setChecked(true);
        }else{
            ui->checkBox_engine_threatcrowd->setChecked(false);
        }
        if(keys_object.value(ENGINE_DNSBUFFEROVERRUN) == OSINT_TRUE){
            ui->checkBox_engine_dnsbufferoverrun->setChecked(true);
        }else{
            ui->checkBox_engine_dnsbufferoverrun->setChecked(false);
        }
        if(keys_object.value(ENGINE_HACKERTARGET) == OSINT_TRUE){
            ui->checkBox_engine_hackertarget->setChecked(true);
        }else{
            ui->checkBox_engine_hackertarget->setChecked(false);
        }
        if(keys_object.value(ENGINE_PKEY) == OSINT_TRUE){
            ui->checkBox_engine_pkey->setChecked(true);
        }else{
            ui->checkBox_engine_pkey->setChecked(false);
        }
        if(keys_object.value(ENGINE_WAYBACKMACHINE) == OSINT_TRUE){
            ui->checkBox_engine_waybackmachine->setChecked(true);
        }else{
            ui->checkBox_engine_waybackmachine->setChecked(false);
        }
        if(keys_object.value(ENGINE_ASK) == OSINT_TRUE){
            ui->checkBox_engine_ask->setChecked(true);
        }else{
            ui->checkBox_engine_ask->setChecked(false);
        }
        if(keys_object.value(ENGINE_BAIDU) == OSINT_TRUE){
            ui->checkBox_engine_baidu->setChecked(true);
        }else{
            ui->checkBox_engine_baidu->setChecked(false);
        }
        if(keys_object.value(ENGINE_DNSDUMPSTER) == OSINT_TRUE){
            ui->checkBox_engine_dnsdumpster->setChecked(true);
        }else{
            ui->checkBox_engine_dnsdumpster->setChecked(false);
        }
        if(keys_object.value(ENGINE_PASSIVEDNS) == OSINT_TRUE){
            ui->checkBox_engine_passivedns->setChecked(true);
        }else{
            ui->checkBox_engine_passivedns->setChecked(false);
        }
        if(keys_object.value(ENGINE_YAHOO) == OSINT_TRUE){
            ui->checkBox_engine_yahoo->setChecked(true);
        }else{
            ui->checkBox_engine_yahoo->setChecked(false);
        }
        if(keys_object.value(ENGINE_VIRUSTOTALAPI) == OSINT_TRUE){
            ui->checkBox_engine_virustotalapi->setChecked(true);
        }else{
            ui->checkBox_engine_virustotalapi->setChecked(false);
        }
        if(keys_object.value(ENGINE_OMNISINT) == OSINT_TRUE){
            ui->checkBox_engine_omnisint->setChecked(true);
        }else{
            ui->checkBox_engine_omnisint->setChecked(false);
        }
        if(keys_object.value(ENGINE_QWANT) == OSINT_TRUE){
            ui->checkBox_engine_qwant->setChecked(true);
        }
        else{
            ui->checkBox_engine_qwant->setChecked(false);
        }
        if(keys_object.value(ENGINE_URLSCAN) == OSINT_TRUE){
            ui->checkBox_engine_urlscan->setChecked(true);
        }else{
            ui->checkBox_engine_urlscan->setChecked(false);
        }
        if(keys_object.value(ENGINE_RAPIDDNS) == OSINT_TRUE){
            ui->checkBox_engine_rapiddns->setChecked(true);
        }
        else{
            ui->checkBox_engine_rapiddns->setChecked(false);
        }
        if(keys_object.value(ENGINE_PROJECTDISCOVERY) == OSINT_TRUE){
            ui->checkBox_engine_projectdiscovery->setChecked(true);
        }
        else{
            ui->checkBox_engine_projectdiscovery->setChecked(false);
        }
        if(keys_object.value(ENGINE_PENTESTTOOLS) == OSINT_TRUE){
            ui->checkBox_engine_pentesttools->setChecked(true);
        }else{
            ui->checkBox_engine_pentesttools->setChecked(false);
        }
        if(keys_object.value(ENGINE_SPYSE) == OSINT_TRUE){
            ui->checkBox_engine_spyse->setChecked(true);
        }else{
            ui->checkBox_engine_spyse->setChecked(false);
        }
    }else{
        logs("[Error] Failed To Open /config/osint-profiles.json File For Loading Profile!");
    }
}
/********************* Deleting a profile ****************************/
void Osint::on_pushButton_deleteProfile_clicked(){
    QFile osintProfiles(currentPath+FILE_PROFILES);
    osintProfiles.open(QIODevice::ReadOnly | QIODevice::Text);
    if(osintProfiles.isOpen()){
        QString keys = osintProfiles.readAll();
        osintProfiles.close();
        QJsonObject keys_object = QJsonDocument::fromJson(keys.toUtf8()).object();
        keys_object.remove(ui->comboBox_profiles->currentText());
        //...
        QJsonDocument JsonDocument;
        JsonDocument.setObject(keys_object);
        osintProfiles.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        if(osintProfiles.isOpen()){
            osintProfiles.write(JsonDocument.toJson());
            osintProfiles.close();
        }
        ui->comboBox_profiles->removeItem(ui->comboBox_profiles->currentIndex());
    }else{
        logs("[Error] Failed To Open /config/osint-profiles.json File For Deleting a Profile!");
    }
}
/***************** creating a new profile *************************/
void Osint::on_pushButton_newProfile_clicked(){
    if(ui->lineEdit_newProfile->text() != EMPTY){
        QString profile_name = ui->lineEdit_newProfile->text();
        ui->comboBox_profiles->addItem(profile_name);
        ui->lineEdit_newProfile->clear();
        ///
        /// saving to profiles...
        ///
        QFile osintProfiles(currentPath+FILE_PROFILES);
        osintProfiles.open(QIODevice::ReadOnly | QIODevice::Text);
        if(osintProfiles.isOpen()){
            ///
            /// reading all data from the key file...
            ///
            QJsonParseError JsonParseError;
            QJsonDocument JsonDocument = QJsonDocument::fromJson(osintProfiles.readAll(), &JsonParseError);
            QJsonObject RootObject = JsonDocument.object();
            osintProfiles.close();
            ///
            /// inserting the user's options into the Json Object...
            ///
            QJsonObject ref_addvalue;
            if(ui->checkBox_engine_censys->isChecked()){
                ref_addvalue.insert(ENGINE_CENSYS, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_CENSYS, OSINT_FALSE);
            }
            if(ui->checkBox_engine_threatminer->isChecked()){
                ref_addvalue.insert(ENGINE_THREATMINER, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_THREATMINER, OSINT_FALSE);
            }
            if(ui->checkBox_engine_shodan->isChecked()){
                ref_addvalue.insert(ENGINE_SHODAN, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_SHODAN, OSINT_FALSE);
            }
            if(ui->checkBox_engine_github->isChecked()){
                ref_addvalue.insert(ENGINE_GITHUB, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_GITHUB, OSINT_FALSE);
            }
            if(ui->checkBox_engine_certspotter->isChecked()){
                ref_addvalue.insert(ENGINE_CERTSPOTTER, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_CERTSPOTTER, OSINT_FALSE);
            }
            if(ui->checkBox_engine_dogpile->isChecked()){
                ref_addvalue.insert(ENGINE_DOGPILE, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_DOGPILE, OSINT_FALSE);
            }
            if(ui->checkBox_engine_duckduckgo->isChecked()){
                ref_addvalue.insert(ENGINE_DUCKDUCKGO, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_DUCKDUCKGO, OSINT_FALSE);
            }
            if(ui->checkBox_engine_exalead->isChecked()){
                ref_addvalue.insert(ENGINE_EXALEAD, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_EXALEAD, OSINT_FALSE);
            }
            if(ui->checkBox_engine_huntersearch->isChecked()){
                ref_addvalue.insert(ENGINE_HUNTERSEARCH, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_HUNTERSEARCH, OSINT_FALSE);
            }
            if(ui->checkBox_engine_intelx->isChecked()){
                ref_addvalue.insert(ENGINE_INTELX, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_INTELX, OSINT_FALSE);
            }
            if(ui->checkBox_engine_netcraft->isChecked()){
                ref_addvalue.insert(ENGINE_NETCRAFT, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_NETCRAFT, OSINT_FALSE);
            }
            if(ui->checkBox_engine_otx->isChecked()){
                ref_addvalue.insert(ENGINE_OTX, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_OTX, OSINT_FALSE);
            }
            if(ui->checkBox_engine_securitytrails->isChecked()){
                ref_addvalue.insert(ENGINE_SECURITYTRAILS, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_SECURITYTRAILS, OSINT_FALSE);
            }
            if(ui->checkBox_engine_suip->isChecked()){
                ref_addvalue.insert(ENGINE_SUIP, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_SUIP, OSINT_FALSE);
            }
            if(ui->checkBox_engine_trello->isChecked()){
                ref_addvalue.insert(ENGINE_TRELLO, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_TRELLO, OSINT_FALSE);
            }
            if(ui->checkBox_engine_san->isChecked()){
                ref_addvalue.insert(ENGINE_SAN, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_SAN, OSINT_FALSE);
            }
            if(ui->checkBox_engine_cloudflare->isChecked()){
                ref_addvalue.insert(ENGINE_CLOUDFLARE, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_CLOUDFLARE, OSINT_FALSE);
            }
            if(ui->checkBox_engine_threatcrowd->isChecked()){
                ref_addvalue.insert(ENGINE_THREATCROWD, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_THREATCROWD, OSINT_FALSE);
            }
            if(ui->checkBox_engine_dnsbufferoverrun->isChecked()){
                ref_addvalue.insert(ENGINE_DNSBUFFEROVERRUN, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_DNSBUFFEROVERRUN, OSINT_FALSE);
            }
            if(ui->checkBox_engine_hackertarget->isChecked()){
                ref_addvalue.insert(ENGINE_HACKERTARGET, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_HACKERTARGET, OSINT_FALSE);
            }
            if(ui->checkBox_engine_pkey->isChecked()){
                ref_addvalue.insert(ENGINE_PKEY, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_PKEY, OSINT_FALSE);
            }
            if(ui->checkBox_engine_waybackmachine->isChecked()){
                ref_addvalue.insert(ENGINE_WAYBACKMACHINE, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_WAYBACKMACHINE, OSINT_FALSE);
            }
            if(ui->checkBox_engine_ask->isChecked()){
                ref_addvalue.insert(ENGINE_ASK, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_ASK, OSINT_FALSE);
            }
            if(ui->checkBox_engine_baidu->isChecked()){
                ref_addvalue.insert(ENGINE_BAIDU, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_BAIDU, OSINT_FALSE);
            }
            if(ui->checkBox_engine_bing->isChecked()){
                ref_addvalue.insert(ENGINE_BING, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_BING, OSINT_FALSE);
            }
            if(ui->checkBox_engine_crtsh->isChecked()){
                ref_addvalue.insert(ENGINE_CRTSH, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_CRTSH, OSINT_FALSE);
            }
            if(ui->checkBox_engine_dnsdumpster->isChecked()){
                ref_addvalue.insert(ENGINE_DNSDUMPSTER, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_DNSDUMPSTER, OSINT_FALSE);
            }
            if(ui->checkBox_engine_google->isChecked()){
                ref_addvalue.insert(ENGINE_GOOGLE, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_GOOGLE, OSINT_FALSE);
            }
            if(ui->checkBox_engine_passivedns->isChecked()){
                ref_addvalue.insert(ENGINE_PASSIVEDNS, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_PASSIVEDNS, OSINT_FALSE);
            }
            if(ui->checkBox_engine_virustotal->isChecked()){
                ref_addvalue.insert(ENGINE_VIRUSTOTAL, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_VIRUSTOTAL, OSINT_FALSE);
            }
            if(ui->checkBox_engine_yahoo->isChecked()){
                ref_addvalue.insert(ENGINE_YAHOO, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_YAHOO, OSINT_FALSE);
            }
            if(ui->checkBox_engine_virustotalapi->isChecked()){
                ref_addvalue.insert(ENGINE_VIRUSTOTALAPI, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_VIRUSTOTALAPI, OSINT_FALSE);
            }
            if(ui->checkBox_engine_omnisint->isChecked()){
                ref_addvalue.insert(ENGINE_OMNISINT, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_OMNISINT, OSINT_FALSE);
            }
            if(ui->checkBox_engine_qwant->isChecked()){
                ref_addvalue.insert(ENGINE_QWANT, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_QWANT, OSINT_FALSE);
            }
            if(ui->checkBox_engine_rapiddns->isChecked()){
                ref_addvalue.insert(ENGINE_RAPIDDNS, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_RAPIDDNS, OSINT_FALSE);
            }
            if(ui->checkBox_engine_urlscan->isChecked()){
                ref_addvalue.insert(ENGINE_URLSCAN, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_URLSCAN, OSINT_FALSE);
            }
            if(ui->checkBox_engine_pentesttools->isChecked()){
                ref_addvalue.insert(ENGINE_PENTESTTOOLS, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_PENTESTTOOLS, OSINT_FALSE);
            }
            if(ui->checkBox_engine_projectdiscovery->isChecked()){
                ref_addvalue.insert(ENGINE_PROJECTDISCOVERY, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_PROJECTDISCOVERY, OSINT_FALSE);
            }
            if(ui->checkBox_engine_spyse->isChecked()){
                ref_addvalue.insert(ENGINE_SPYSE, OSINT_TRUE);
            }else{
                ref_addvalue.insert(ENGINE_SPYSE, OSINT_FALSE);
            }
            RootObject.insert(profile_name, ref_addvalue);
            JsonDocument.setObject(RootObject);
            osintProfiles.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            if(osintProfiles.isOpen()){
                osintProfiles.write(JsonDocument.toJson());
                osintProfiles.close();
            }else{
                logs("[Error] Failed To Open /config/osint-profiles.json File For Saving New Profile!");
            }
        }else{
            QMessageBox::warning(this, TITLE_ERROR, "Please Enter New Profile's Name...");
        }
    }
}


/*************************** Action-Button Context Menu **************************************/
void Osint::on_pushButton_action_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(m_resultsModel->osint->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->pushButton_action->mapToGlobal(QPoint(0,0));
    contextMenu_actionButton(ENGINE::OSINT, pos);
}

/******************************** Right-Click Context Menu ****************************/
void Osint::on_tableView_results_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableView_results->selectionModel()->isSelected(ui->tableView_results->currentIndex())){
        return;
    }
    contextMenu_rightClick(ui->tableView_results->selectionModel());
}

void Osint::on_comboBox_target_currentIndexChanged(int index){
    if(index){
        ui->frame_targets->show();
    }
    else{
        ui->frame_targets->hide();
    }
}
