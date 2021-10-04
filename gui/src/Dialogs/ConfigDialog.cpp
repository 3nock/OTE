#include "ConfigDialog.h"
#include "ui_ConfigDialog.h"
//...
#include "src/utils/Config.h"


/*
 * TODO: add option to view and choose default nameservers you want to use...
 *       add site's own nameservers...
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
    int size = Config::generalConfig().beginReadArray("Custom-Nameservers");
    for(int i = 0; i < size; i++){
        Config::generalConfig().setArrayIndex(i);
        ui->customNameservers->add(Config::generalConfig().value("value").toString());
    }
    Config::generalConfig().endArray();
    ///
    /// other specific configurations...
    ///
    Config::generalConfig().beginGroup(m_scanConfig->name);
    ui->lineEditThreads->setText(Config::generalConfig().value("threads").toString());
    ui->lineEditTimeout->setText(Config::generalConfig().value("timeout").toString());
    Config::generalConfig().endGroup();
}

void ConfigDialog::saveConfigValues(){
    ///
    /// saving the custom nameservers...
    ///
    Config::generalConfig().beginWriteArray("Custom-Nameservers");
    for(int i = 0; i < ui->customNameservers->listModel->rowCount(); i++){
        Config::generalConfig().setArrayIndex(i);
        Config::generalConfig().setValue("value", ui->customNameservers->listModel->stringList().at(i));
    }
    Config::generalConfig().endArray();
    ///
    /// saving the custom nameservers...
    ///
    Config::generalConfig().beginGroup(m_scanConfig->name);
    Config::generalConfig().setValue("threads", ui->lineEditThreads->text());
    Config::generalConfig().setValue("timeout", ui->lineEditTimeout->text());
    Config::generalConfig().endGroup();
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
    if(ui->checkBoxCustomNameServers->isChecked() && ui->customNameservers->listModel->rowCount() > 0)
    {
        for(int i = 0; i != ui->customNameservers->listModel->rowCount(); ++i)
            m_scanConfig->customNameServers.append(ui->customNameservers->listModel->stringList().at(i));
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
