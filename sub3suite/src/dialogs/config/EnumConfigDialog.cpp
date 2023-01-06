#include "EnumConfigDialog.h"
#include "ui_EnumConfigDialog.h"

#include "src/utils/Config.h"


EnumConfigDialog::EnumConfigDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::EnumConfigDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/gear.png"));

    ui->lineEditTimeout->setPlaceholderText("e.g. 1000");
}
EnumConfigDialog::~EnumConfigDialog(){
    delete ui;
}

void EnumConfigDialog::on_buttonOk_clicked(){
    mSaveConfig();
    accept();
}

void EnumConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void EnumConfigDialog::mLoadConfig(){
    ui->lineEditTimeout->setText(QString::number(gConfig.enums.timeout));
    ui->checkBoxAutosave->setChecked(gConfig.enums.autoSaveResults);
    ui->checkBoxNoDuplicates->setChecked(gConfig.enums.noDuplicates);
    ui->groupBoxTimeout->setChecked(gConfig.enums.useTimeout);
}

void EnumConfigDialog::mSaveConfig(){
    gConfig.enums.timeout = ui->lineEditTimeout->text().toInt();
    gConfig.enums.autoSaveResults = ui->checkBoxAutosave->isChecked();
    gConfig.enums.noDuplicates = ui->checkBoxNoDuplicates->isChecked();
    gConfig.enums.useTimeout = ui->groupBoxTimeout->isChecked();
}
