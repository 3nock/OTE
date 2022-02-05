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

    accept();
}

void EnumConfigDialog::on_buttonCancel_clicked(){
    accept();
}

///
/// loading configurations from config files...
///
void EnumConfigDialog::loadConfig_asn(){
    ui->checkBoxAutosave->setChecked(CONFIG_ENUM.value("autosave_to_Project_asn").toBool());
    ui->checkBoxNoDuplicates->setChecked(CONFIG_ENUM.value("no_duplicates_asn").toBool());

    asn = true;
}

///
/// saving the configurations
///
void EnumConfigDialog::saveConfig_asn(){
    /* save to config file */
    CONFIG_ENUM.setValue("autosave_to_Project_asn", ui->checkBoxAutosave->isChecked());
    CONFIG_ENUM.setValue("no_duplicates_asn", ui->checkBoxNoDuplicates->isChecked());

    /* save to config structure */
    m_config->autosaveToProject = ui->checkBoxAutosave->isChecked();
    m_config->noDuplicates = ui->checkBoxNoDuplicates->isChecked();
}
