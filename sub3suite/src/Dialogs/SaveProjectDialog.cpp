#include "SaveProjectDialog.h"
#include "ui_SaveProjectDialog.h"

#include "src/utils/Config.h"

#include <QDir>
#include <QFileDialog>
#include <QMessageBox>


SaveProjectDialog::SaveProjectDialog(ProjectModel *projectModel, QWidget *parent) : QDialog(parent),
    ui(new Ui::SaveProjectDialog),
    m_projectModel(projectModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/save.png"));

    /* setting the data */
    ui->lineEditName->setText(m_projectModel->projectInfo.name);

    if(m_projectModel->projectInfo.path.isEmpty())
        ui->lineEditPath->setText(CONFIG.value("projects_path").toString());
    else{
        QString path = m_projectModel->projectInfo.path.remove(m_projectModel->projectInfo.name+".s3s");
        ui->lineEditPath->setText(path);
    }
}
SaveProjectDialog::~SaveProjectDialog(){
    delete ui;
}

void SaveProjectDialog::on_buttonSave_clicked(){
    /* get data */
    QString path = ui->lineEditPath->text();
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

    QString prev_name = m_projectModel->projectInfo.name;
    QString prev_path = m_projectModel->projectInfo.path;

    /* setting new copy details */
    m_projectModel->projectInfo.name = name;
    m_projectModel->projectInfo.path = path+"/"+name+".s3s";

    qDebug() << "Saving a project copy: " << m_projectModel->projectInfo.path;
    m_projectModel->saveProject();
    qDebug() << "Saving project copy done!";

    /* restoring default details */
    m_projectModel->projectInfo.name = prev_name;
    m_projectModel->projectInfo.path = prev_path;

    accept();
}

void SaveProjectDialog::on_buttonCancel_clicked(){
    accept();
}

void SaveProjectDialog::on_buttonChoosePath_clicked(){
    QString path = QFileDialog::getExistingDirectory(this, "Path", ui->lineEditPath->text());
    if(path.isEmpty()){
        return;
    }
    ui->lineEditPath->setText(path);
}
