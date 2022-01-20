#include "StartupDialog.h"
#include "ui_StartupDialog.h"

#include "AboutDialog.h"


StartupDialog::StartupDialog(QString *project, QWidget *parent) : QDialog(parent),
    ui(new Ui::StartupDialog),
    m_project(project)
{
    ui->setupUi(this);

    /* setting up logo */
    QPixmap logo_sub3suite(":/img/res/icons/sub3suite.png");
    logo_sub3suite.scaled(ui->labelS3S->width(), ui->labelS3S->height());
    ui->labelS3S->setPixmap(logo_sub3suite);
    ui->labelS3S->setScaledContents(true);
}
StartupDialog::~StartupDialog(){
    delete ui;
}

void StartupDialog::on_buttonOpen_clicked(){
    if(ui->checkBoxTemporary->isChecked())
        m_project->append("Temp");

    accept();
}

void StartupDialog::on_buttonCancel_clicked(){
    accept();
}

void StartupDialog::on_checkBoxTemporary_clicked(bool checked){
    if(checked){
        ui->groupBoxNewProject->setChecked(false);
        ui->groupBoxExistingProject->setChecked(false);
    }
}

void StartupDialog::on_groupBoxNewProject_toggled(bool checked){
    if(checked){
        ui->checkBoxTemporary->setChecked(false);
        ui->groupBoxExistingProject->setChecked(false);
    }
}

void StartupDialog::on_groupBoxExistingProject_toggled(bool checked){
    if(checked){
        ui->groupBoxNewProject->setChecked(false);
        ui->checkBoxTemporary->setChecked(false);
    }
}
