#include "ConfigDialog.h"
#include "ui_ConfigDialog.h"
//...
#include "src/Config.h"


/*
 * TODO:
 *
 */
ConfigDialog::ConfigDialog(QWidget *parent, ScanConfig *scanConfig) : QDialog(parent), ui(new Ui::ConfigDialog),
      m_scanConfig(scanConfig)
{
    ui->setupUi(this);
    ///
    /// initializing...
    ///
    ui->customNameservers->init("NS");
    ///
    /// hiding widgets for custom nameservers input untill user chooses
    /// to use custom nameservers...
    ///
    ui->customNameservers->hide();
    ConfigDialog::adjustSize();
    ///
    /// setting the placeholder text...
    ///
    ui->lineEditThreads->setPlaceholderText("Thread count...");
    ui->lineEditTimeout->setPlaceholderText("Lookup timeout...");
    ///
    /// load the config values...
    ///
    this->loadConfigValues();
}
ConfigDialog::~ConfigDialog(){
    delete ui;
}

void ConfigDialog::loadConfigValues(){
    ///
    /// getting custom nameservers...
    ///
    int size = Config::settings().beginReadArray("Custom-Nameservers");
    for(int i = 0; i < size; i++){
        Config::settings().setArrayIndex(i);
        ui->customNameservers->add(Config::settings().value("value").toString());
    }
    Config::settings().endArray();
    ///
    /// other specific configurations...
    ///
    Config::settings().beginGroup(m_scanConfig->name);
    ui->lineEditThreads->setText(Config::settings().value("threads").toString());
    ui->lineEditTimeout->setText(Config::settings().value("timeout").toString());
    Config::settings().endGroup();
}

void ConfigDialog::saveConfigValues(){
    ///
    /// saving the custom nameservers...
    ///
    Config::settings().beginWriteArray("Custom-Nameservers");
    for(int i = 0; i < ui->customNameservers->listWidget->count(); i++){
        Config::settings().setArrayIndex(i);
        Config::settings().setValue("value", ui->customNameservers->listWidget->item(i)->text());
    }
    Config::settings().endArray();
    ///
    /// saving the custom nameservers...
    ///
    Config::settings().beginGroup(m_scanConfig->name);
    Config::settings().setValue("threads", ui->lineEditThreads->text());
    Config::settings().setValue("timeout", ui->lineEditTimeout->text());
    Config::settings().endGroup();
}

void ConfigDialog::on_buttonOk_clicked(){
    if(ui->lineEditThreads->text().toInt() <= 100 && ui->lineEditThreads->text().toInt() > 0){
        m_scanConfig->threadsCount = ui->lineEditThreads->text().toInt();
    }
    if(ui->radioButtonDnsRecordTypeA->isChecked()){
        m_scanConfig->dnsRecordType = QDnsLookup::A;
    }
    if(ui->radioButtonDnsRecordTypeAAAA->isChecked()){
        m_scanConfig->dnsRecordType = QDnsLookup::AAAA;
    }
    if(ui->checkBoxCheckWildcards->isChecked()){
        m_scanConfig->checkWildcard = true;
    }
    if(ui->checkBoxCustomNameServers->isChecked() && ui->customNameservers->listWidget->count() > 0)
    {
        for(int i = 0; i != ui->customNameservers->listWidget->count(); ++i)
            m_scanConfig->customNameServers.append(ui->customNameservers->listWidget->item(i)->text());
        m_scanConfig->useCustomNameServers = true;
    }
    else{
        m_scanConfig->useCustomNameServers = false;
    }
    this->saveConfigValues();
    accept();
}

void ConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void ConfigDialog::on_checkBoxCustomNameServers_clicked(bool checked){
    if(checked)
        ui->customNameservers->show();
    else
        ui->customNameservers->hide();
    ConfigDialog::adjustSize();
}
