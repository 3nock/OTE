#include "EnumConfigDialog.h"
#include "ui_EnumConfigDialog.h"

#include "src/utils/Config.h"


EnumConfigDialog::EnumConfigDialog(QWidget *parent, ScanConfig *config) : QDialog(parent),
    ui(new Ui::EnumConfigDialog),
    m_config(config)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->lineEditTimeout->setPlaceholderText("e.g. 1000");

    CONFIG.beginGroup(CFG_ENUM);
}
EnumConfigDialog::~EnumConfigDialog(){
    CONFIG.endGroup();
    delete ui;
}

void EnumConfigDialog::on_buttonOk_clicked(){
    if(asn)
        this->saveConfig_asn();
    if(cidr)
        this->saveConfig_cidr();
    if(ip)
        this->saveConfig_ip();
    if(ns)
        this->saveConfig_ns();
    if(mx)
        this->saveConfig_mx();
    if(ssl)
        this->saveConfig_ssl();
    if(email)
        this->saveConfig_email();

    accept();
}

void EnumConfigDialog::on_buttonCancel_clicked(){
    accept();
}

///
/// loading configurations from config files...
///
void EnumConfigDialog::loadConfig_asn(){
    ui->lineEditTimeout->setText(CONFIG.value("timout_asn").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_asn").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_asn").toBool());

    asn = true;
}

void EnumConfigDialog::loadConfig_cidr(){
    ui->lineEditTimeout->setText(CONFIG.value("timout_cidr").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_cidr").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_cidr").toBool());

    cidr = true;
}

void EnumConfigDialog::loadConfig_ip(){
    ui->lineEditTimeout->setText(CONFIG.value("timout_ip").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_ip").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_ip").toBool());

    ip = true;
}

void EnumConfigDialog::loadConfig_ns(){
    ui->lineEditTimeout->setText(CONFIG.value("timout_ns").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_ns").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_ns").toBool());

    ns = true;
}

void EnumConfigDialog::loadConfig_mx(){
    ui->lineEditTimeout->setText(CONFIG.value("timout_mx").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_mx").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_mx").toBool());

    mx = true;
}

void EnumConfigDialog::loadConfig_ssl(){
    ui->lineEditTimeout->setText(CONFIG.value("timout_ssl").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_ssl").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_ssl").toBool());

    ssl = true;
}

void EnumConfigDialog::loadConfig_email(){
    ui->lineEditTimeout->setText(CONFIG.value("timout_email").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_email").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_email").toBool());

    email = true;
}

///
/// saving the configurations
///
void EnumConfigDialog::saveConfig_asn(){
    /* save to config file */
    CONFIG.setValue("autosave_to_Project_asn", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_asn", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_asn", ui->lineEditTimeout->text());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_cidr(){
    /* save to config file */
    CONFIG.setValue("autosave_to_Project_cidr", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_cidr", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_cidr", ui->lineEditTimeout->text());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_ip(){
    /* save to config file */
    CONFIG.setValue("autosave_to_Project_ip", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_ip", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_ip", ui->lineEditTimeout->text());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_ns(){
    /* save to config file */
    CONFIG.setValue("autosave_to_Project_ns", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_ns", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_ns", ui->lineEditTimeout->text());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_mx(){
    /* save to config file */
    CONFIG.setValue("autosave_to_Project_mx", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_mx", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_mx", ui->lineEditTimeout->text());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_ssl(){
    /* save to config file */
    CONFIG.setValue("autosave_to_Project_ssl", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_ssl", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_ssl", ui->lineEditTimeout->text());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_email(){
    /* save to config file */
    CONFIG.setValue("autosave_to_Project_email", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_email", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_email", ui->lineEditTimeout->text());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}
