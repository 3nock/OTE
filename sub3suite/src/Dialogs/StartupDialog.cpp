#include "StartupDialog.h"
#include "ui_StartupDialog.h"

#include "src/utils/Config.h"

#include <QDir>
#include <QDebug>
#include <QFileInfo>


StartupDialog::StartupDialog(QString *project, QWidget *parent) : QDialog(parent),
    ui(new Ui::StartupDialog),
    m_project(project),
    existing_model(new QStandardItemModel)
{
    ui->setupUi(this);

    /* setting up logo */
    QPixmap logo_sub3suite(":/img/res/icons/sub3suite.png");
    logo_sub3suite.scaled(ui->labelS3S->width(), ui->labelS3S->height());
    ui->labelS3S->setPixmap(logo_sub3suite);
    ui->labelS3S->setScaledContents(true);

    /* ... */
    existing_model->setHorizontalHeaderLabels({"Name", "File"});
    ui->tableViewProjects->setModel(existing_model);

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

        QFileInfo fileInfo(file.fileName());

        /* add file to list */
        existing_model->invisibleRootItem()->appendRow({new QStandardItem(fileInfo.fileName().split(".")[0]),
                                                       new QStandardItem(projectfile)});
    }
    CONFIG.endGroup();
}
StartupDialog::~StartupDialog(){
    delete ui;
}

void StartupDialog::on_buttonOpen_clicked(){
    if(ui->radioButtonTemporary->isChecked())
        m_project->append("Temp");

    /*
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
    */

    accept();
}

void StartupDialog::on_buttonCancel_clicked(){
    accept();
}
