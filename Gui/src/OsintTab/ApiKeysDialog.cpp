#include "ApiKeysDialog.h"
#include "ui_ApiKeysDialog.h"


ApiKeysDialog::ApiKeysDialog(QWidget *parent) :QDialog(parent),ui(new Ui::ApiKeysDialog){
    ui->setupUi(this);
    currentPath = QDir::currentPath();
    QFile api_keys(currentPath+FILE_APIKEYS);
    api_keys.open(QIODevice::ReadOnly | QIODevice::Text);
    if(api_keys.isOpen()){
        try{
            // reading all data from the key file...
            QString keys = api_keys.readAll();
            api_keys.close();
            QJsonDocument apis = QJsonDocument::fromJson(keys.toUtf8());
            QJsonObject keys_object = apis.object()["api_keys"].toObject();
            // appending the keys data to the GUI Line Edits...
            if(!keys_object.value(ENGINE_BING).toString().isEmpty()){
                ui->lineEdit_bing->setText(keys_object.value(ENGINE_BING).toString());
                ui->toolButton_getApiKey_bing->hide();
            }
            if(!keys_object.value(ENGINE_GITHUB).toString().isEmpty()){
                ui->lineEdit_github->setText(keys_object.value(ENGINE_GITHUB).toString());
                ui->toolButton_getApiKey_github->hide();
            }
            if(!keys_object.value(ENGINE_SPYSE).toString().isEmpty()){
                ui->lineEdit_spyse->setText(keys_object.value(ENGINE_SPYSE).toString());
                ui->toolButton_getApiKey_spyse->hide();
            }
            if(!keys_object.value(ENGINE_SHODAN).toString().isEmpty()){
                ui->lineEdit_shodan->setText(keys_object.value(ENGINE_SHODAN).toString());
                ui->toolButton_getApiKey_shodan->hide();
            }
            if(!keys_object.value(ENGINE_SECURITYTRAILS).toString().isEmpty()){
                ui->lineEdit_trailssec->setText(keys_object.value(ENGINE_SECURITYTRAILS).toString());
                ui->toolButton_getApiKey_securityTrails->hide();
            }
            if(!keys_object.value(ENGINE_CLOUDFLARE).toString().isEmpty()){
                ui->lineEdit_cloudflare->setText(keys_object.value(ENGINE_CLOUDFLARE).toString());
                ui->toolButton_getApiKey_cloudflare->hide();
            }
            if(!keys_object.value(ENGINE_INTELX).toString().isEmpty()){
                ui->lineEdit_intelx->setText(keys_object.value(ENGINE_INTELX).toString());
                ui->toolButton_getApiKey_intelx->hide();
            }
            if(!keys_object.value(ENGINE_VIRUSTOTALAPI).toString().isEmpty()){
                ui->lineEdit_virustotal->setText(keys_object.value(ENGINE_VIRUSTOTALAPI).toString());
                ui->toolButton_getApiKey_virustotal->hide();
            }
            if(!keys_object.value(ENGINE_GOOGLE).toString().isEmpty()){
                ui->lineEdit_google->setText(keys_object.value(ENGINE_GOOGLE).toString());
                ui->toolButton_getApiKey_google->hide();
            }
            if(!keys_object.value(ENGINE_HUNTERSEARCH).toString().isEmpty()){
                ui->lineEdit_hunter->setText(keys_object.value(ENGINE_HUNTERSEARCH).toString());
                ui->toolButton_getApiKey_hunter->hide();
            }
            if(!keys_object.value(ENGINE_CERTSPOTTER).toString().isEmpty()){
                ui->lineEdit_certspotter->setText(keys_object.value(ENGINE_CERTSPOTTER).toString());
                ui->toolButton_getApiKey_certspotter->hide();
            }
            if(!keys_object.value(ENGINE_PROJECTDISCOVERY).toString().isEmpty()){
                ui->lineEdit_projectdiscovery->setText(keys_object.value(ENGINE_PROJECTDISCOVERY).toString());
                ui->toolButton_getApiKey_projectdiscovery->hide();
            }
            if(!keys_object.value(ENGINE_PENTESTTOOLS).toString().isEmpty()){
                ui->lineEdit_pentesttools->setText(keys_object.value(ENGINE_PENTESTTOOLS).toString());
                ui->toolButton_getApiKey_pentesttools->hide();
            }
            if(!keys_object.value("riskiq_username").toString().isEmpty() && !keys_object.value("riskiq_key").toString().isEmpty()){
                ui->lineEdit_riskiq_username->setText(keys_object.value("riskiq_username").toString());
                ui->lineEdit_riskiq_key->setText(keys_object.value("riskiq_key").toString());
                ui->toolButton_getApiKey_riskiq->hide();
            }
            if(!keys_object.value("censys_secret").toString().isEmpty() && !keys_object.value("censys_id").toString().isEmpty()){
                ui->lineEdit_censys_secret->setText(keys_object.value("censys_secret").toString());
                ui->lineEdit_censys_id->setText(keys_object.value("censys_id").toString());
                ui->toolButton_getApiKey_censys->hide();
            }
        }
        catch(...){
            QMessageBox::warning(this, TITLE_ERROR, "An Exception occurred While reading the api-keys From osint-apiKeys.json File!");
        }
    }else{
        QMessageBox::warning(this, TITLE_ERROR, "An Error Occurred Opening osint-apiKeys.json File!");
    }
}
ApiKeysDialog::~ApiKeysDialog(){
    delete ui;
}

void ApiKeysDialog::on_pushButton_save_clicked(){
    QFile api_keys(currentPath+FILE_APIKEYS);
    api_keys.open(QIODevice::ReadOnly | QIODevice::Text);
    if(api_keys.isOpen()){
        try{
            // reading all data from the key file...
            QJsonParseError JsonParseError;
            QJsonDocument JsonDocument = QJsonDocument::fromJson(api_keys.readAll(), &JsonParseError);
            QJsonObject RootObject = JsonDocument.object();
            api_keys.close();
            // write the data to the files....
            QJsonValueRef ref = RootObject.find("api_keys").value();
            QJsonObject ref_addvalue = ref.toObject();
            ref_addvalue.insert(ENGINE_BING, ui->lineEdit_bing->text());
            ref_addvalue.insert(ENGINE_INTELX, ui->lineEdit_intelx->text());
            ref_addvalue.insert(ENGINE_SECURITYTRAILS, ui->lineEdit_trailssec->text());
            ref_addvalue.insert(ENGINE_SHODAN, ui->lineEdit_shodan->text());
            ref_addvalue.insert(ENGINE_SPYSE, ui->lineEdit_spyse->text());
            ref_addvalue.insert(ENGINE_CLOUDFLARE, ui->lineEdit_cloudflare->text());
            ref_addvalue.insert(ENGINE_GITHUB, ui->lineEdit_github->text());
            ref_addvalue.insert(ENGINE_GOOGLE, ui->lineEdit_google->text());
            ref_addvalue.insert(ENGINE_VIRUSTOTALAPI, ui->lineEdit_virustotal->text());
            ref_addvalue.insert(ENGINE_HUNTERSEARCH, ui->lineEdit_hunter->text());
            ref_addvalue.insert(ENGINE_CERTSPOTTER, ui->lineEdit_certspotter->text());
            ref_addvalue.insert(ENGINE_PROJECTDISCOVERY, ui->lineEdit_projectdiscovery->text());
            ref_addvalue.insert(ENGINE_PENTESTTOOLS, ui->lineEdit_pentesttools->text());
            ref_addvalue.insert("riskiq_username", ui->lineEdit_riskiq_username->text());
            ref_addvalue.insert("riskiq_key", ui->lineEdit_riskiq_key->text());
            ref_addvalue.insert("censys_id", ui->lineEdit_censys_id->text());
            ref_addvalue.insert("censys_secret", ui->lineEdit_censys_secret->text());
            ref = ref_addvalue;
            JsonDocument.setObject(RootObject);
            api_keys.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
            if(api_keys.isOpen()){
                api_keys.write(JsonDocument.toJson());
                api_keys.close();
            }else{
                QMessageBox::warning(this, TITLE_ERROR, "Failed To Open osint-apiKeys.json File For Saving api-keys!");
            }
            QMessageBox::information(this, "Done!", "Api keys Saved!");
        }catch(...){
            QMessageBox::warning(this, TITLE_ERROR, "An Exception Occurred While writing to osint-apiKeys.json File");
        }
    }else{
        QMessageBox::warning(this, TITLE_ERROR, "Failed To Open osint-apiKeys.json File!");
    }
    accept();
}
void ApiKeysDialog::on_pushButton_cancel_clicked(){
    accept();
}

/*************************** ACQUIRE API KEYS FROM RESPECTIVE SITES *******************************/
void ApiKeysDialog::on_toolButton_getApiKey_censys_clicked(){
    QDesktopServices::openUrl(QUrl("https://censys.io/login", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_virustotal_clicked(){
     QDesktopServices::openUrl(QUrl("https://www.virustotal.com/gui/join-us", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_github_clicked(){
    QDesktopServices::openUrl(QUrl("https://github.com/settings/tokens", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_shodan_clicked(){
    QDesktopServices::openUrl(QUrl("https://developer.shodan.io/api/requirements", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_cloudflare_clicked(){
    QDesktopServices::openUrl(QUrl("https://dash.cloudflare.com/profile/api-tokens", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_bing_clicked(){
    QDesktopServices::openUrl(QUrl("https://www.microsoft.com/en-us/bing/apis/bing-web-search-api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_google_clicked(){
    QDesktopServices::openUrl(QUrl("https://developers.google.com/maps/documentation/places/web-service/get-api-key", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_hunter_clicked(){
    QDesktopServices::openUrl(QUrl("https://hunter.io/api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_certspotter_clicked(){
    QDesktopServices::openUrl(QUrl("https://sslmate.com/certspotter/api/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_intelx_clicked(){
    QDesktopServices::openUrl(QUrl("https://intelx.io/signup", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_securityTrails_clicked(){
    QDesktopServices::openUrl(QUrl("https://securitytrails.com/corp/api", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_spyse_clicked(){
    QDesktopServices::openUrl(QUrl("https://spyse.com/user/registration", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_riskiq_clicked(){
    QDesktopServices::openUrl(QUrl("https://api.riskiq.net/api/", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_pentesttools_clicked(){
    QDesktopServices::openUrl(QUrl("https://pentest-tools.com/pricing", QUrl::TolerantMode));
}

void ApiKeysDialog::on_toolButton_getApiKey_projectdiscovery_clicked(){
    QDesktopServices::openUrl(QUrl("https://chaos.projectdiscovery.io/", QUrl::TolerantMode));
}
