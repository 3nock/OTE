#include "StartupDialog.h"
#include "ui_StartupDialog.h"

#include "src/utils/Config.h"

#include <QDir>
#include <QDebug>


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

    /* ... */
    ui->lineEditLocation->setText(QDir::currentPath());

    /* load recent projects */
    CONFIG.beginGroup("recent_projects");
    QStringList keys = CONFIG.allKeys();
    foreach(const QString &key, keys){
        QString projectfile = CONFIG.value(key).toString();

        /* check if project file exists if it doesnt delete in recents */
        QFile file(projectfile);
        if(!file.exists()){
            qDebug() << "Project File: " << projectfile << " doesnt exists. Deleting from existing Projects";
            CONFIG.remove(key);
        }

        /* add file to listwidget */
        ui->listWidgetProjects->addItem(projectfile);
    }
    CONFIG.endGroup();
}
StartupDialog::~StartupDialog(){
    delete ui;
}

void StartupDialog::on_buttonOpen_clicked(){
    if(ui->checkBoxTemporary->isChecked())
        m_project->append("Temp");

    if(ui->groupBoxNewProject->isChecked()){
        QString file = ui->lineEditLocation->text();
        file.append("/");
        m_project->append(file+ui->lineEditName->text());
    }

    if(ui->groupBoxExistingProject->isChecked()){
        if(ui->listWidgetProjects->selectedItems().isEmpty())
            m_project->append("Temp");
        else
            m_project->append(ui->listWidgetProjects->selectedItems().at(0)->text());
    }

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
