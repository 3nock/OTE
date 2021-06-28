#include "Osint.h"
#include "ui_Osint.h"

/******************************* Constructor & Destructor ********************************/

Osint::Osint(QWidget *parent) : QWidget(parent), ui(new Ui::Osint),
    results_model(new QStandardItemModel),
    //...
    scanArguments(new ScanArguments_Osint),
    scanResults(new ScanResults_Osint)
{
    ui->setupUi(this);
    //...
    currentPath = QDir::currentPath();
    ui->pushButton_stop->setDisabled(true);
    ui->lineEdit_domain->setPlaceholderText("eg. example.com");
    ui->lineEdit_newProfile->setPlaceholderText("Enter New Profile's Name...");
    //...
    results_model->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
    ui->tableView_results->setModel(results_model);
    //...
    ui->progressBar->hide();
    //...
    QPalette p = palette();
    p.setColor(QPalette::Highlight, QColor(188, 188, 141));
    p.setColor(QPalette::HighlightedText, QColor(Qt::black));
    ui->tableView_results->setPalette(p);
    //...
    setupOsintProfiles();
}
Osint::~Osint(){
    delete results_model;
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
        m_targetDomain = TargetNameFilter(ui->lineEdit_domain->text(), ENUMNAME_OSINT);
        ///
        /// converting the QString domainName to char* for compatibility with the PyObject methods...
        ///
        char *targetDomain = new char[m_targetDomain.length() + 1];
        strcpy(targetDomain, m_targetDomain.toStdString().c_str());
        //...
        scanArguments->targetDomain = targetDomain;
        scanResults->label_subdomainsCount = ui->label_subdomainsCount;
        scanResults->resultsCount = &subdomainsCount;
        scanResults->results_model = results_model;
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
        connect(this, SIGNAL(stopEnumeration()), enumerator, SLOT(onStopEnumeration()));
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
    emit stopEnumeration();
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
        results_model->clear();
        ui->label_subdomainsCount->clear();
        subdomainsCount = 0;
        //...
        results_model->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
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
    OsintConfigDialog *scanConfig = new OsintConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}

/******************************************************************
                     OSINT ENGINES OPTIONS
*******************************************************************/
void Osint::getUserOptions(QStringList *choosenEngines){
    if(ui->checkBox_engine_threatminer->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_THREATMINER);
    }
    if(ui->checkBox_engine_shodan->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_SHODAN);
    }
    if(ui->checkBox_engine_otx->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_OTX);
    }
    if(ui->checkBox_engine_netcraft->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_NETCRAFT);
    }
    if(ui->checkBox_engine_censys->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_CENSYS);
    }
    if(ui->checkBox_engine_github->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_GITHUB);
    }
    if(ui->checkBox_engine_certspotter->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_CERTSPOTTER);
    }
    if(ui->checkBox_engine_dogpile->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_DOGPILE);
    }
    if(ui->checkBox_engine_duckduckgo->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_DUCKDUCKGO);
    }
    if(ui->checkBox_engine_exalead->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_EXALEAD);
    }
    if(ui->checkBox_engine_huntersearch->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_HUNTERSEARCH);
    }
    if(ui->checkBox_engine_intelx->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_INTELX);
    }
    if(ui->checkBox_engine_securitytrails->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_SECURITYTRAILS);
    }
    if(ui->checkBox_engine_suip->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_SUIP);
    }
    if(ui->checkBox_engine_trello->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_TRELLO);
    }
    if(ui->checkBox_engine_san->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_SAN);
    }
    if(ui->checkBox_engine_cloudflare->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_CLOUDFLARE);
    }
    if(ui->checkBox_engine_threatcrowd->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_THREATCROWD);
    }
    if(ui->checkBox_engine_dnsbufferoverrun->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_DNSBUFFEROVERRUN);
    }
    if(ui->checkBox_engine_hackertarget->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_HACKERTARGET);
    }
    if(ui->checkBox_engine_pkey->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_PKEY);
    }
    if(ui->checkBox_engine_waybackmachine->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_WAYBACKMACHINE);
    }
    if(ui->checkBox_engine_ask->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_ASK);
    }
    if(ui->checkBox_engine_baidu->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_BAIDU);
    }
    if(ui->checkBox_engine_bing->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_BING);
    }
    if(ui->checkBox_engine_crtsh->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_CRTSH);
    }
    if(ui->checkBox_engine_dnsdumpster->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_DNSDUMPSTER);
    }
    if(ui->checkBox_engine_google->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_GOOGLE);
    }
    if(ui->checkBox_engine_passivedns->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_PASSIVEDNS);
    }
    if(ui->checkBox_engine_virustotal->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_VIRUSTOTAL);
    }
    if(ui->checkBox_engine_yahoo->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_YAHOO);
    }
    if(ui->checkBox_engine_virustotalapi->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_VIRUSTOTALAPI);
    }
    if(ui->checkBox_engine_omnisint->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_OMNISINT);
    }
    if(ui->checkBox_engine_qwant->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_QWANT);
    }
    if(ui->checkBox_engine_rapiddns->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_RAPIDDNS);
    }
    if(ui->checkBox_engine_urlscan->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_URLSCAN);
    }
    if(ui->checkBox_engine_pentesttools->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_PENTESTTOOLS);
    }
    if(ui->checkBox_engine_projectdiscovery->isChecked() || ui->radioButton_fullScan->isChecked()){
        choosenEngines->append(ENGINE_PROJECTDISCOVERY);
    }
    if(ui->checkBox_engine_spyse->isChecked() || ui->radioButton_fullScan->isChecked()){
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
    QMenu *menu = new QMenu(this);
    menu->setAttribute( Qt::WA_DeleteOnClose, true );
    menu->setObjectName("mainMenu");
    //...
    QAction actionSendToSave("Send To Save", this);
    QAction actionSendToActive("Send To Active Subdomain", this);
    QAction actionSendToDnsRecords("Send To DnsRecords");
    QAction actionRemoveDuplicates("Remove Duplicates");
    //...
    connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(actionSendToSave()));
    connect(&actionSendToActive, SIGNAL(triggered()), this, SLOT(actionSendToActive()));
    connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(actionSendToDnsRecords()));
    connect(&actionRemoveDuplicates, SIGNAL(triggered()), this,SLOT(actionRemoveDuplicates()));
    //...
    menu->addAction(&actionRemoveDuplicates);
    menu->addSeparator();
    menu->addAction(&actionSendToDnsRecords);
    menu->addAction(&actionSendToSave);
    menu->addAction(&actionSendToActive);
    ///
    /// positioning of the context menu...
    ///
    QPoint pos = ui->pushButton_action->mapToGlobal(QPoint(0,0));
    int x = pos.x()+76;
    int y = pos.y();
    //...
    menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
    menu->move(QPoint(x, y));
    menu->exec();
}

/******************************** Right-Click Context Menu ****************************/
void Osint::on_tableView_results_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    if(ui->tableView_results->selectionModel()->isSelected(ui->tableView_results->currentIndex())){
        QMenu *menu = new QMenu(this);
        menu->setAttribute( Qt::WA_DeleteOnClose, true );
        menu->setObjectName("mainMenu");
        //...
        QAction actionSendToSave("Send Selected To Save", this);
        QAction actionSendToDnsRecords("Send Selected To DnsRecords", this);
        QAction actionSendToInfo("Send Selected To Info");
        QAction actionOpenInBrowser("Open Selected in Browser");
        //...
        connect(&actionOpenInBrowser, SIGNAL(triggered()), this, SLOT(cursorOpenInBrowser()));
        connect(&actionSendToSave, SIGNAL(triggered()), this, SLOT(cursorSendToSave()));
        connect(&actionSendToDnsRecords, SIGNAL(triggered()), this, SLOT(cursorSendToDnsRecords()));
        connect(&actionSendToInfo, SIGNAL(triggered()), this, SLOT(cursorSendToInfo()));
        //...
        menu->addAction(&actionOpenInBrowser);
        menu->addSeparator();
        menu->addAction(&actionSendToInfo);
        menu->addAction(&actionSendToSave);
        menu->addAction(&actionSendToDnsRecords);
        ///
        /// getting the mouse position..
        ///
        QPoint globalCursorPos = QCursor::pos();
        int mouseScreen = qApp->desktop()->screenNumber(globalCursorPos);
        QRect mouseScreenGeometry = qApp->desktop()->screen(mouseScreen)->geometry();
        QPoint localCursorPosition = globalCursorPos - mouseScreenGeometry.topLeft();
        //...
        menu->setStyleSheet("QMenu::item::selected#mainMenu{background-color: rgb(170, 170, 127)} QMenu#mainMenu{background-color: qlineargradient(x1:0,  y1:0, x2:0, y2:1, stop: 0 white, stop: 0.8 rgb(246, 255, 199)); border-style: solid; border-color: black; border-width: 1px;}");
        menu->move(localCursorPosition);
        menu->exec();
    }
}

/************************ Action-Button Context Menu Methods ***********************/
void Osint::actionSendToSave(){
    onSendResultsToSave();
    emit changeTabToSave();
}
void Osint::actionSendToActive(){
    onSendResultsToActive();
    emit changeTabToActive();
}
void Osint::actionRemoveDuplicates(){
    // removing duplicates from the QListWidget...
}
void Osint::actionSendToDnsRecords(){
    // nothing yet...
}

/************************** Right-Click Context Menu Methods **********************/
void Osint::cursorOpenInBrowser(){
    ///
    /// iterate and open each selected item in a browser...
    ///
    foreach(const QModelIndex &index, ui->tableView_results->selectionModel()->selectedIndexes()){
        QDesktopServices::openUrl(QUrl("https://"+index.data().toString(), QUrl::TolerantMode));
    }
}
void Osint::cursorSendToSave(){
    foreach(const QModelIndex &index, ui->tableView_results->selectionModel()->selectedIndexes()){
        emit sendResultsToSave(index.data().toString());
    }
    emit changeTabToSave();
    logs("[*] Sent Selected Osint Results To Save...");
}
void Osint::cursorSendToActive(){
    foreach(const QModelIndex &index, ui->tableView_results->selectionModel()->selectedIndexes()){
        emit sendResultsToActive(index.data().toString());
    }
    emit changeTabToActive();
    logs("[*] Sent Selected Osint Results To enumerate Active Subdomains in Active...");
}
void Osint::cursorSendToDnsRecords(){
    // not yet implemented...
}

/*********************************** logs ***************************************/
void Osint::logs(QString log){
    sendLog(log);
    ui->listWidget_logs->addItem(log);
    if(log.startsWith("[Error]") || log.startsWith("[Exception]")){
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setForeground(Qt::red);
        return;
    }
    if(log.startsWith("[START]") || log.startsWith("[END]")){
        ui->listWidget_logs->item(ui->listWidget_logs->count()-1)->setFont(QFont("MS Shell Dlg 2", 8, QFont::Bold));
        return;
    }
}

/********************************* Send Results **********************************/
void Osint::onSendResultsToSave(){
    //...
    logs("[*] Sent Osint Results To Save...");
}
void Osint::onSendResultsToActive(){
    //...
    logs("[*] Sent Osint Results To enumerate Active Subdomains...");
}
