#include "Osint.h"
#include "ui_Osint.h"
//...
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>

Osint::Osint(QWidget *parent, ResultsModel *resultsModel) : BaseClass(ENGINE::OSINT, resultsModel, parent), ui(new Ui::Osint),
    scanArguments(new osint::ScanArguments),
    scanResults(new osint::ScanResults)
{
    ui->setupUi(this);
    ///
    /// init...
    ///
    ui->targets->init("Targets");
    initBaseClass(ui->targets);
    scanConfig->name = tr("ScanConfig-Osint");
    ///
    /// ...
    ///
    currentPath = QDir::currentPath();
    ui->lineEditTarget->setPlaceholderText("eg. example.com");
    ui->lineEditNewProfile->setPlaceholderText("Enter New Profile's Name...");
    //...
    resultsModel->osint->setHorizontalHeaderLabels({"Subdomain", "IpAddress"});
    ui->tableViewResults->setModel(resultsModel->osint);
    //...
    ui->buttonPause->setDisabled(true);
    ui->buttonStop->setDisabled(true);
    ///
    /// hide widgets...
    ///
    ui->progressBar->hide();
    ui->targets->hide();
    ///
    /// equally seperate the widgets...
    ///
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    //...
    setupOsintProfiles();
}
Osint::~Osint(){
    delete scanArguments;
    delete scanResults;
    delete ui;
}

void Osint::on_buttonStart_clicked(){
    if(ui->lineEditTarget->text() != EMPTY){
        getUserOptions(&scanArguments->choosenOptions);
        if(scanArguments->choosenOptions.count() == 0){
            QMessageBox::warning(this, "Error!", "Please Choose Osint Engine For subdomain Enumerations!");
            return;
        }
        if(ui->lineEditTarget->text().isEmpty()){
            QMessageBox::warning(this, "Error!", "Please Target Domain For Enumerations!");
            return;
        }
        //...
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
        m_targetDomain = TargetNameFilter(ui->lineEditTarget->text(), ENGINE::OSINT);
        ///
        /// converting the QString domainName to char* for compatibility with the PyObject methods...
        ///
        char *targetDomain = new char[m_targetDomain.length() + 1];
        strcpy(targetDomain, m_targetDomain.toStdString().c_str());
        //...
        scanArguments->targetDomain = targetDomain;
        scanResults->label_subdomainsCount = ui->labelResultsCount;
        scanResults->resultsCount = &subdomainsCount;
        scanResults->results_model = resultsModel->osint;
        //...
        emit sendStatus("[*] Enumerating "+m_targetDomain+" Subdomains with Osint...");
        logs("[START] Enumerating "+m_targetDomain+" Subdomains with Osint...");
        //...
        osint::Scanner *scanner = new osint::Scanner(scanArguments, scanResults);
        QThread *cThread = new QThread(this);
        scanner->startScan(cThread);
        scanner->moveToThread(cThread);
        //...
        connect(scanner, SIGNAL(scanLog(QString)), this, SLOT(logs(QString)));
        connect(cThread, SIGNAL(finished()), this, SLOT(onEnumerationComplete()));
        connect(cThread, SIGNAL(finished()), scanner, SLOT(deleteLater()));
        connect(cThread, SIGNAL(finished()), cThread, SLOT(deleteLater()));
        connect(this, SIGNAL(stopScan()), scanner, SLOT(onStopEnumeration()));
        //...
        cThread->start();
    }else{
        QMessageBox::warning(this, "Error!", "Please Enter Target Domain Name to Enumerate Subdomains!");
    }
}
void Osint::on_lineEditTarget_returnPressed(){
    on_buttonStart_clicked();
}

void Osint::on_buttonStop_clicked(){
    emit stopScan();
    sendStatus("[*] Stopping...");
}

void Osint::onEnumerationComplete(){
    //...
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
    //...
    emit sendStatus("[END] Enumeration Complete!");
    logs("[END] Enumeration Complete!\n");
}

void Osint::on_buttonClear_clicked(){
    // clear subdomains...
    if(ui->tabWidgetResults->currentIndex() == 0){
        resultsModel->osint->clear();
        ui->labelResultsCount->clear();
        subdomainsCount = 0;
        //...
        resultsModel->osint->setHorizontalHeaderLabels({"Subdomain Name", "IpAddress"});
    }
    // clear logs...
    else{
        ui->listWidgetLogs->clear();
    }
}

void Osint::on_buttonKeys_clicked(){
    ApiKeysDialog *apiKeysDialog = new ApiKeysDialog(this);
    apiKeysDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeysDialog->show();
}
void Osint::on_buttonConfig_clicked(){
    /*
    OsintConfigDialog *scanConfig = new OsintConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
    */
}

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
            ui->comboBoxProfiles->addItem(profile_names[i]);
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
void Osint::on_checkBoxUseProfiles_clicked(bool checked){
    if(checked){
        ui->frame_profiles->show();
    }else{
        ui->frame_profiles->hide();
    }
}

void Osint::on_buttonLoadProfile_clicked(){
    QFile osintProfiles(currentPath+FILE_PROFILES);
    osintProfiles.open(QIODevice::ReadOnly | QIODevice::Text);
    if(osintProfiles.isOpen()){
        QString keys = osintProfiles.readAll();
        osintProfiles.close();
        QJsonDocument apis = QJsonDocument::fromJson(keys.toUtf8());
        QJsonObject keys_object = apis.object()[ui->comboBoxProfiles->currentText()].toObject();
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

void Osint::on_buttonDeleteProfile_clicked(){
    QFile osintProfiles(currentPath+FILE_PROFILES);
    osintProfiles.open(QIODevice::ReadOnly | QIODevice::Text);
    if(osintProfiles.isOpen()){
        QString keys = osintProfiles.readAll();
        osintProfiles.close();
        QJsonObject keys_object = QJsonDocument::fromJson(keys.toUtf8()).object();
        keys_object.remove(ui->comboBoxProfiles->currentText());
        //...
        QJsonDocument JsonDocument;
        JsonDocument.setObject(keys_object);
        osintProfiles.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
        if(osintProfiles.isOpen()){
            osintProfiles.write(JsonDocument.toJson());
            osintProfiles.close();
        }
        ui->comboBoxProfiles->removeItem(ui->comboBoxProfiles->currentIndex());
    }else{
        logs("[Error] Failed To Open /config/osint-profiles.json File For Deleting a Profile!");
    }
}

void Osint::on_buttonNewProfile_clicked(){
    if(ui->lineEditNewProfile->text() != EMPTY){
        QString profile_name = ui->lineEditNewProfile->text();
        ui->comboBoxProfiles->addItem(profile_name);
        ui->lineEditNewProfile->clear();
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
            QMessageBox::warning(this, "Error!", "Please Enter New Profile's Name...");
        }
    }
}

void Osint::on_buttonAction_clicked(){
    ///
    /// check if there are results available else dont show the context menu...
    ///
    if(resultsModel->osint->rowCount() < 1){
        return;
    }
    ///
    /// getting the position of the action button to place the context menu and
    /// showing the context menu right by the side of the action button...
    ///
    QPoint pos = ui->buttonAction->mapToGlobal(QPoint(0,0));
    a_Menu->exec(QPoint(pos.x()+76, pos.y()));
}

void Osint::on_tableViewResults_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);
    ///
    /// check if user right clicked on items else dont show the context menu...
    ///
    if(!ui->tableViewResults->selectionModel()->isSelected(ui->tableViewResults->currentIndex())){
        return;
    }
    selectionModel = ui->tableViewResults->selectionModel();
    c_Menu->exec(QCursor::pos());
}

void Osint::on_checkBoxMultipleTargets_clicked(bool checked){
    if(checked)
        ui->targets->show();
    else
        ui->targets->hide();
}
