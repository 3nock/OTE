#include "ProjectConfigDialog.h"
#include "ui_ProjectConfigDialog.h"

#include "src/utils/Config.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>


ProjectConfigDialog::ProjectConfigDialog(ProjectModel *projectModel, QWidget *parent) :QDialog(parent),
    ui(new Ui::ProjectConfigDialog),
    m_projectModel(projectModel)
{
    ui->setupUi(this);

    /* setting the data */
    ui->lineEditName->setText(m_projectModel->projectInfo.name);

    if(m_projectModel->projectInfo.path.isEmpty())
        ui->lineEditFile->setText(CONFIG.value("projects_path").toString());
    else{
        QString path = m_projectModel->projectInfo.path.remove(m_projectModel->projectInfo.name+".s3s");
        ui->lineEditFile->setText(path);
    }
}
ProjectConfigDialog::~ProjectConfigDialog(){
    delete ui;
}

void ProjectConfigDialog::on_buttonCancel_clicked(){
    accept();
}

void ProjectConfigDialog::on_buttonOk_clicked(){
    /* get data */
    QString path = ui->lineEditFile->text();
    QString name = ui->lineEditName->text();

    /* checks */
    if(path.isEmpty() || name.isEmpty()){
        QMessageBox::warning(this, "Error", "Please fill-in the details!");
        return;
    }
    if(!QDir::isAbsolutePath(path)){
        QMessageBox::warning(this, "Error", "Wrong Path For Project File!");
        return;
    }

    m_projectModel->projectInfo.name = name;
    m_projectModel->projectInfo.path = path+"/"+name+".s3s";

    /* checks */
    m_projectModel->project_explorer->setText(m_projectModel->projectInfo.name);
    accept();
}

void ProjectConfigDialog::on_buttonChoosePath_clicked(){
    QString path = QFileDialog::getExistingDirectory(this, "Path", ui->lineEditFile->text());
    if(path.isEmpty()){
        return;
    }
    ui->lineEditFile->setText(path);
}
