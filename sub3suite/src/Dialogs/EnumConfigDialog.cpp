#include "EnumConfigDialog.h"
#include "ui_EnumConfigDialog.h"

#include "src/utils/Config.h"


EnumConfigDialog::EnumConfigDialog(QWidget *parent, ScanConfig *config) : QDialog(parent),
    ui(new Ui::EnumConfigDialog),
    m_config(config)
{
    ui->setupUi(this);
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

    accept();
}

void EnumConfigDialog::on_buttonCancel_clicked(){
    accept();
}

///
/// loading configurations from config files...
///
void EnumConfigDialog::loadConfig_asn(){
    ui->checkBoxAutosave->setChecked(CONFIG_ENUM.value("autosave_to_Project_cidr").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_ENUM.value("no_duplicates_cidr").toBool());

    asn = true;
}

void EnumConfigDialog::loadConfig_cidr(){
    ui->checkBoxAutosave->setChecked(CONFIG_ENUM.value("autosave_to_Project_cidr").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_ENUM.value("no_duplicates_cidr").toBool());

    cidr = true;
}

void EnumConfigDialog::loadConfig_ip(){
    ui->checkBoxAutosave->setChecked(CONFIG_ENUM.value("autosave_to_Project_ip").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_ENUM.value("no_duplicates_ip").toBool());

    ip = true;
}

///
/// saving the configurations
///
void EnumConfigDialog::saveConfig_asn(){
    /* save to config file */
    CONFIG_ENUM.setValue("autosave_to_Project_cidr", ui->checkBoxAutosave->isChecked());
    CONFIG_ENUM.setValue("no_duplicates_cidr", ui->checkBoxNoDuplicates->isChecked());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
}

void EnumConfigDialog::saveConfig_cidr(){
    /* save to config file */
    CONFIG_ENUM.setValue("autosave_to_Project_cidr", ui->checkBoxAutosave->isChecked());
    CONFIG_ENUM.setValue("no_duplicates_cidr", ui->checkBoxNoDuplicates->isChecked());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
}

void EnumConfigDialog::saveConfig_ip(){
    /* save to config file */
    CONFIG_ENUM.setValue("autosave_to_Project_ip", ui->checkBoxAutosave->isChecked());
    CONFIG_ENUM.setValue("no_duplicates_ip", ui->checkBoxNoDuplicates->isChecked());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
}
