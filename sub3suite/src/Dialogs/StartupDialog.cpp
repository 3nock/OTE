#include "StartupDialog.h"
#include "ui_StartupDialog.h"

#include "src/utils/Config.h"
#include "src/dialogs/AboutDialog.h"

#include <QUrl>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QFileDialog>
#include <QDesktopServices>


StartupDialog::StartupDialog(ProjectStruct *project, QWidget *parent) : QDialog(parent),
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

    /* github account & twitter */
    s3s_ClickableLabel *githubLabel = new s3s_ClickableLabel("", this);
    QPixmap github_logo(":/img/res/icons/github.png");
    githubLabel->setPixmap(github_logo.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    s3s_ClickableLabel *twitterLabel = new s3s_ClickableLabel("", this);
    QPixmap twitter_logo(":/img/res/icons/twitter.png");
    twitterLabel->setPixmap(twitter_logo.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->horizontalLayout_s3s->insertWidget(0, githubLabel);
    ui->horizontalLayout_s3s->insertWidget(1, twitterLabel);

    connect(githubLabel, &s3s_ClickableLabel::clicked, this, [=](){
        QDesktopServices::openUrl(QUrl("https://github.com/3nock/sub3suite", QUrl::TolerantMode));
    });
    connect(twitterLabel, &s3s_ClickableLabel::clicked, this, [=](){
        QDesktopServices::openUrl(QUrl("https://twitter.com/sub3suite", QUrl::TolerantMode));
    });
}
StartupDialog::~StartupDialog(){
    delete existing_model;
    delete ui;
}

void StartupDialog::on_buttonOpen_clicked(){
    if(ui->radioButtonTemporary->isChecked())
        m_project->isTemporary = true;

    if(ui->radioButtonNewProject->isChecked())
    {
        if(!ui->lineEditName->text().isEmpty()){
            QString projectName = ui->lineEditName->text();
            m_project->name = projectName;
            m_project->path = QGuiApplication::applicationDirPath()+"/projects/"+projectName+".s3s";
            m_project->isNew = true;
            accept();
            return;
        }
        if(!ui->lineEditLocation->text().isEmpty()){
            QFile file(ui->lineEditLocation->text());
            if(file.exists())
            {
                QFileInfo fileInfo(file);
                m_project->name = fileInfo.fileName().split(".")[0];
                m_project->path = file.fileName();
                m_project->isNew = true;
            }
            else
                m_project->isTemporary = true;
        }
    }

    if(ui->radioButtonExistingProject->isChecked())
    {
        QItemSelectionModel *selection = ui->tableViewProjects->selectionModel();
        if(selection->selectedIndexes().isEmpty())
        {
            if(!ui->lineEditExisting->text().isEmpty()){
                QFile file(ui->lineEditExisting->text());
                if(file.exists())
                {
                    QFileInfo fileInfo(file);
                    m_project->name = fileInfo.fileName().split(".")[0];
                    m_project->path = file.fileName();
                    m_project->isExisting = true;
                }
            }else
                m_project->isTemporary = true;
        }
        else{
            m_project->name = selection->selectedIndexes()[0].data().toString();
            m_project->path = selection->selectedIndexes()[1].data().toString();
            m_project->isExisting = true;
        }
    }

    accept();
}

void StartupDialog::on_buttonCancel_clicked(){
    accept();
}

void StartupDialog::on_buttonAbout_clicked(){
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    aboutDialog->show();
}

void StartupDialog::on_tableViewProjects_pressed(const QModelIndex &index){
    Q_UNUSED(index);

    ui->radioButtonExistingProject->setChecked(true);
}

void StartupDialog::on_buttonChooseNew_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, "Open", ui->lineEditLocation->text(), "*.s3s");
    if(filename.isEmpty()){
        return;
    }
}

void StartupDialog::on_buttonChooseExisting_clicked(){
    QString filename = QFileDialog::getOpenFileName(this, "Open", ui->lineEditExisting->text(), "*.s3s");
    if(filename.isEmpty()){
        return;
    }
}
