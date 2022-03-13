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
}
EnumConfigDialog::~EnumConfigDialog(){
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
    CONFIG.beginGroup(CFG_ENUM);
    ui->lineEditTimeout->setText(CONFIG.value("timeout_asn").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_asn").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_asn").toBool());
    CONFIG.endGroup();

    asn = true;
}

void EnumConfigDialog::loadConfig_cidr(){
    CONFIG.beginGroup(CFG_ENUM);
    ui->lineEditTimeout->setText(CONFIG.value("timeout_cidr").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_cidr").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_cidr").toBool());
    CONFIG.endGroup();

    cidr = true;
}

void EnumConfigDialog::loadConfig_ip(){
    CONFIG.beginGroup(CFG_ENUM);
    ui->lineEditTimeout->setText(CONFIG.value("timeout_ip").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_ip").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_ip").toBool());
    CONFIG.endGroup();

    ip = true;
}

void EnumConfigDialog::loadConfig_ns(){
    CONFIG.beginGroup(CFG_ENUM);
    ui->lineEditTimeout->setText(CONFIG.value("timeout_ns").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_ns").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_ns").toBool());
    CONFIG.endGroup();

    ns = true;
}

void EnumConfigDialog::loadConfig_mx(){
    CONFIG.beginGroup(CFG_ENUM);
    ui->lineEditTimeout->setText(CONFIG.value("timeout_mx").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_mx").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_mx").toBool());
    CONFIG.endGroup();

    mx = true;
}

void EnumConfigDialog::loadConfig_ssl(){
    CONFIG.beginGroup(CFG_ENUM);
    ui->lineEditTimeout->setText(CONFIG.value("timeout_ssl").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_ssl").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_ssl").toBool());
    CONFIG.endGroup();

    ssl = true;
}

void EnumConfigDialog::loadConfig_email(){
    CONFIG.beginGroup(CFG_ENUM);
    ui->lineEditTimeout->setText(CONFIG.value("timeout_email").toString());
    ui->checkBoxAutosave->setChecked(CONFIG.value("autosave_to_Project_email").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG.value("no_duplicates_email").toBool());
    CONFIG.endGroup();

    email = true;
}

///
/// saving the configurations
///
void EnumConfigDialog::saveConfig_asn(){
    /* save to config file */
    CONFIG.beginGroup(CFG_ENUM);
    CONFIG.setValue("autosave_to_Project_asn", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_asn", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_asn", ui->lineEditTimeout->text());
    CONFIG.endGroup();

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_cidr(){
    /* save to config file */
    CONFIG.beginGroup(CFG_ENUM);
    CONFIG.setValue("autosave_to_Project_cidr", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_cidr", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_cidr", ui->lineEditTimeout->text());
    CONFIG.endGroup();

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_ip(){
    /* save to config file */
    CONFIG.beginGroup(CFG_ENUM);
    CONFIG.setValue("autosave_to_Project_ip", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_ip", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_ip", ui->lineEditTimeout->text());
    CONFIG.endGroup();

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_ns(){
    /* save to config file */
    CONFIG.beginGroup(CFG_ENUM);
    CONFIG.setValue("autosave_to_Project_ns", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_ns", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_ns", ui->lineEditTimeout->text());
    CONFIG.endGroup();

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_mx(){
    /* save to config file */
    CONFIG.beginGroup(CFG_ENUM);
    CONFIG.setValue("autosave_to_Project_mx", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_mx", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_mx", ui->lineEditTimeout->text());
    CONFIG.endGroup();

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_ssl(){
    /* save to config file */
    CONFIG.beginGroup(CFG_ENUM);
    CONFIG.setValue("autosave_to_Project_ssl", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_ssl", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_ssl", ui->lineEditTimeout->text());
    CONFIG.endGroup();

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}

void EnumConfigDialog::saveConfig_email(){
    /* save to config file */
    CONFIG.beginGroup(CFG_ENUM);
    CONFIG.setValue("autosave_to_Project_email", ui->checkBoxAutosave->isChecked());
    CONFIG.setValue("no_duplicates_email", ui->checkBoxNoDuplicates->isChecked());
    CONFIG.setValue("timeout_email", ui->lineEditTimeout->text());
    CONFIG.endGroup();

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    m_config->timeout = ui->lineEditTimeout->text().toInt();
}
