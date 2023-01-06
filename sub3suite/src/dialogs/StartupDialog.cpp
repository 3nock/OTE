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
#include <QMessageBox>


StartupDialog::StartupDialog(ProjectStruct *project, QWidget *parent) : QDialog(parent),
    ui(new Ui::StartupDialog),
    m_project(project),
    model_existing(new QStandardItemModel)
{
    this->initUI();

    model_existing->setHorizontalHeaderLabels({tr(" Name"), tr(" File")});
    ui->tableViewProjects->setModel(model_existing);

    // projects path
    ui->lineEditNew_path->setText(QGuiApplication::applicationDirPath()+"/projects");

    // load recent projects
    foreach(const QString &project, gConfig.recents.keys())
    {
        QString projectfile = gConfig.recents[project];

        // check if project file exists if it doesnt delete in recents
        QFile file(projectfile);
        if(!file.exists()){
            qWarning() << "Project File: " << projectfile << " doesnt exists. Deleting from existing Projects";
            gConfig.recents.remove(project);
        }
        else {
            QFileInfo fileInfo(file.fileName());
            model_existing->invisibleRootItem()->appendRow({new QStandardItem(fileInfo.fileName().split(".")[0]),
                                                            new QStandardItem(projectfile)});
        }
    }

    /* default option */
    ui->radioButtonTemporary->setChecked(true);
}
StartupDialog::~StartupDialog(){
    delete model_existing;
    delete ui;
}

void StartupDialog::initUI(){
    ui->setupUi(this);

    /* sub3suite logo */
    QPixmap logo_sub3suite(":/img/res/icons/sub3suite.png");
    logo_sub3suite.scaled(ui->labelS3S->width(), ui->labelS3S->height());
    ui->labelS3S->setPixmap(logo_sub3suite);
    ui->labelS3S->setScaledContents(true);

    /* github & twitter clickable labels */
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

void StartupDialog::on_buttonOpen_clicked(){
    if(ui->radioButtonTemporary->isChecked())
    {
        m_project->name = "Temp";
        m_project->path = QGuiApplication::applicationDirPath()+"/projects/Temp.s3s";
        m_project->isTemporary = true;

        accept();
        return;
    }

    if(ui->radioButtonNewProject->isChecked())
    {
        QString projectName = ui->lineEditNew_name->text();
        if(projectName.isEmpty()){
            QMessageBox::warning(this, "Error!", "Please Enter Name for the New Project!");
            return;
        }

        QString projectPath = ui->lineEditNew_path->text();
        QDir dir(projectPath);
        if(!dir.exists()){
            QMessageBox::warning(this, "Error!", "Please provide a valid path for the New project!");
            return;
        }

        if(!projectPath.endsWith("/"))
            projectPath.append("/");

        m_project->name = projectName;
        m_project->path = projectPath+projectName+".s3s";
        m_project->isNew = true;

        accept();
        return;
    }

    if(ui->radioButtonExistingProject->isChecked())
    {
        QItemSelectionModel *selection = ui->tableViewProjects->selectionModel();
        if(selection->selectedIndexes().isEmpty())
        {
            QString project = ui->lineEditExisting_file->text();
            if(project.isEmpty()){
                QMessageBox::warning(this, "Error!", "Please choose the project to open!");
                return;
            }

            if(!project.endsWith(".s3s")){
                QMessageBox::warning(this, "Error!", "Please choose a valid project file to open!");
                return;
            }

            QFile file(project);
            if(!file.exists()){
                QMessageBox::warning(this, "Error!", "Please choose a valid project file to open!");
                return;
            }

            QFileInfo fileInfo(file);
            m_project->name = fileInfo.fileName().split(".")[0];
            m_project->path = file.fileName();
            m_project->isExisting = true;
        }
        else{
            m_project->name = selection->selectedIndexes()[0].data().toString();
            m_project->path = selection->selectedIndexes()[1].data().toString();
            m_project->isExisting = true;
        }

        accept();
        return;
    }
}

void StartupDialog::on_buttonCancel_clicked(){
    accept();
}

void StartupDialog::on_buttonAbout_clicked(){
    AboutDialog *aboutDialog = new AboutDialog(this);
    aboutDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    aboutDialog->show();
}

void StartupDialog::on_buttonChooseNew_clicked(){
    ui->radioButtonNewProject->setChecked(true);
    QString path = QFileDialog::getExistingDirectory(this, "Choose Path", ui->lineEditNew_path->text());
    if(path.isEmpty())
        return;

    ui->lineEditNew_path->setText(path);
}

void StartupDialog::on_buttonChooseExisting_clicked(){
    ui->radioButtonExistingProject->setChecked(true);
    QString filename = QFileDialog::getOpenFileName(this, "Choose File", ui->lineEditExisting_file->text(), "*.s3s");
    if(filename.isEmpty())
        return;

    ui->lineEditExisting_file->setText(filename);
    ui->tableViewProjects->clearSelection();
}

///
/// radiobutton change...
///

void StartupDialog::on_tableViewProjects_pressed(const QModelIndex &index){
    Q_UNUSED(index);

    ui->radioButtonExistingProject->setChecked(true);
}

void StartupDialog::on_lineEditExisting_file_textChanged(const QString &arg1){
    Q_UNUSED(arg1);

    ui->radioButtonExistingProject->setChecked(true);
}

void StartupDialog::on_lineEditNew_name_textChanged(const QString &arg1){
    Q_UNUSED(arg1);

    ui->radioButtonNewProject->setChecked(true);
}

void StartupDialog::on_lineEditNew_path_textChanged(const QString &arg1){
    Q_UNUSED(arg1);

    ui->radioButtonNewProject->setChecked(true);
}
