//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QCloseEvent>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QMessageBox>
#include <QStyle>
#include <QtConcurrent/QtConcurrent>

#include "src/core/Config.h"
#include "src/dialogs/TemplateDialog.h"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mUpdateChecker(new OTE::UpdateChecker),
    // database
    mDatabase(new OTE::Database),
    // dialogs
    mAboutDialog(new AboutDialog(this)),
    mApiKeysDialog(new ApiKeysDialog(this)),
    mLogViewerDialog(new LogViewerDialog(this)),
    mPreferencesDialog(new PreferencesDialog(this)),
    // views
    mExplorerView(new ExplorerView(this)),
    mExtractorView(new ExtractorView(this)),
    mWelcomeView(new WelcomeView(this)),
    // menus
    mMenuRecents(new QMenu(this))
{
    ui->setupUi(this);

    // actions
    ui->actionExit->setIcon(QIcon(":/icons/res/icons/exit.png"));
    ui->actionAbout->setIcon(QIcon(":/icons/res/icons/about.png"));
    ui->actionDonate->setIcon(QIcon(":/icons/res/icons/donate.png"));
    ui->actionAbout_Qt->setIcon(QIcon(":/icons/res/icons/qt-logo.png"));
    ui->actionTwitter->setIcon(QIcon(":/icons/res/icons/twitter.png"));
    ui->actionGithub->setIcon(QIcon(":/icons/res/icons/github.png"));
    ui->actionWebsite->setIcon(QIcon(":/icons/res/icons/website.png"));
    ui->actionOpen->setIcon(QIcon(":/icons/res/icons/open.png"));
    ui->actionSave->setIcon(QIcon(":/icons/res/icons/save.png"));
    ui->actionClear->setIcon(QIcon(":/icons/res/icons/delete.png"));
    ui->actionOpenRecents->setIcon(QIcon(":/icons/res/icons/recent.png"));
    ui->actionCheckUpdates->setIcon(QIcon(":/icons/res/icons/updates.png"));
    ui->actionPreferences->setIcon(QIcon(":/icons/res/icons/gear.png"));
    ui->actionClose->setIcon(QIcon(":/icons/res/icons/close.png"));
    ui->actionLogViewer->setIcon(QIcon(":/icons/res/icons/logs.png"));
    ui->actionApiKeys->setIcon(QIcon(":/icons/res/icons/key.png"));
    ui->actionNewTemplate->setIcon(QIcon(":/icons/res/icons/add.png"));

    connect(this, &MainWindow::newTemplate, mExplorerView, &ExplorerView::onNewTemplate);
    connect(this, &MainWindow::newTemplate, mExtractorView, &ExtractorView::onNewTemplate);
    connect(this, &MainWindow::newTemplate, mApiKeysDialog, &ApiKeysDialog::onNewTemplate);
    connect(this, &MainWindow::clear, mExplorerView, &ExplorerView::onClear);
    connect(this, &MainWindow::clear, mExtractorView, &ExtractorView::onClear);
    connect(this, &MainWindow::newProject, mExplorerView, &ExplorerView::onNewProject);
    connect(this, &MainWindow::newProject, mExtractorView, &ExtractorView::onNewProject);

    connect(mApiKeysDialog, &ApiKeysDialog::keysUpdated, mExplorerView, &ExplorerView::onKeysUpdated);
    connect(mApiKeysDialog, &ApiKeysDialog::keysUpdated, mExtractorView, &ExtractorView::onKeysUpdated);

    ui->stackedWidget->addWidget(mWelcomeView);
    ui->stackedWidget->addWidget(mExplorerView);
    ui->stackedWidget->addWidget(mExtractorView);

    if(!OTE::gConfig.general.recents.isEmpty())
    {
        foreach(const QString &project_name, OTE::gConfig.general.recents.keys())
        {
            if(QFileInfo::exists(OTE::gConfig.general.recents[project_name]))
            {
                mMenuRecents->addAction(project_name, this, [=](){
                    if(QFileInfo::exists(OTE::gConfig.general.recents.value(project_name)))
                        this->openProject(OTE::gConfig.general.recents.value(project_name));
                    else
                        QMessageBox::warning(this, tr("Error"), tr("Project: %1 no longer exists").arg(OTE::gConfig.general.recents.value(project_name)));
                });
            }
        }
        ui->actionOpenRecents->setMenu(mMenuRecents);
    }

    QAction *aWelcomeView = ui->sidebar->addAction(tr("Welcome"), QIcon(":/icons/res/icons/grid.png"));
    QAction *aExplorerView = ui->sidebar->addAction(tr("Explorer"), QIcon(":/icons/res/icons/raw.png"));
    QAction *aExtractorView = ui->sidebar->addAction(tr("Extractor"), QIcon(":/icons/res/icons/extractor.png"));

    connect(aWelcomeView, &QAction::triggered, this, [=]{
        ui->stackedWidget->setCurrentIndex(0);
    });
    connect(aExplorerView, &QAction::triggered, this, [=]{
        ui->stackedWidget->setCurrentIndex(1);
    });
    connect(aExtractorView, &QAction::triggered, this, [=]{
        ui->stackedWidget->setCurrentIndex(2);
    });

    ui->sidebar->click(0);

#if defined (Q_OS_LINUX)
    this->setWindowIcon(Utility::getApplicationIcon());
#endif

    // check for updates
    if(!OTE::gConfig.general.disableAutomaticUpdateCheck)
        mUpdateChecker->checkForUpdates_onStart();
}

MainWindow::~MainWindow()
{
    // save configurations
    OTE::gConfig.toConfigFile();

    delete mMenuRecents;
    delete mWelcomeView;
    delete mExtractorView;
    delete mExplorerView;
    delete mPreferencesDialog;
    delete mLogViewerDialog;
    delete mApiKeysDialog;
    delete mAboutDialog;
    delete mDatabase;
    delete mUpdateChecker;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int retVal = QMessageBox::warning(nullptr,
                                      tr("Exit"),
                                      tr("Do you want to close SpiderSuite OSINT Tool?"),
                                      QMessageBox::Close |
                                      QMessageBox::Cancel,
                                      QMessageBox::Close);

    if(retVal == QMessageBox::Cancel)
        event->ignore();
}

void MainWindow::openProject(const QString &project)
{
    // clear views
    emit clear();

    // set new project database
    OTE::Database::openDatabase(project);

    // add project to recents
    QString projectName = QFileInfo(project).fileName();
    if(!OTE::gConfig.general.recents.contains(projectName))
    {
        OTE::gConfig.general.recents.insert(projectName, project);

        if(QFileInfo::exists(OTE::gConfig.general.recents[projectName]))
        {
            mMenuRecents->addAction(projectName, this, [=](){
                this->openProject(OTE::gConfig.general.recents.value(projectName));
            });
        }
    }

    emit newProject();
}

void MainWindow::createNewTemplate()
{
    OTE::Template *tmplt = new OTE::Template;

    TemplateDialog templateDialog(this, tmplt);

    if(templateDialog.exec() == QDialog::Accepted)
    {
        QString filename(QCoreApplication::applicationDirPath());
        filename.append("/templates/").append(tmplt->info.name).append(".json");

        QFile file(filename);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(OTE::Template::ToJSON(tmplt));
            file.close();

            OTE::gTemplates << tmplt;

            emit newTemplate(tmplt);

            QMessageBox::information(this, tr("Done"), tr("New OSINT Template %1 successfully created!").arg(tmplt->info.name));
        }
        else
            qWarning() << "Failed To Create File: " << filename;
    }
    else
        delete tmplt;
}

void MainWindow::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open"), OTE::gConfig.general.recentDirectory, "*.otepd");
    if(filename.isEmpty())
        return;

    QFile file(filename);
    if(file.exists())
    {
        emit clear();
        this->openProject(filename);
        OTE::gConfig.general.recentDirectory = QFileInfo(file).dir().path();
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString project = QFileDialog::getSaveFileName(this, tr("Project Name"), OTE::gConfig.general.recentDirectory, "*.otepd");
    if(project.isEmpty())
        return;

    QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>();
    watcher->connect(watcher, &QFutureWatcher<bool>::finished, this, [=](){
        QFuture<bool> future = watcher->future();
        if(future.result())
        {
            qDebug() << "Project saved successfully";
            QMessageBox::information(this, tr("Info"), tr("Project Saved Successfully..."));

            QString projectName = QFileInfo(project).fileName();
            if(!OTE::gConfig.general.recents.contains(projectName)){
                OTE::gConfig.general.recents.insert(projectName, project);

                if(QFileInfo::exists(OTE::gConfig.general.recents[projectName]))
                {
                    mMenuRecents->addAction(projectName, this, [=](){
                        this->openProject(OTE::gConfig.general.recents.value(projectName));
                    });
                }
            }
        }
        watcher->deleteLater();
    });

    watcher->setFuture(QtConcurrent::run(OTE::Database::saveDatabase, project));
}

void MainWindow::on_actionClear_triggered()
{

}

void MainWindow::on_actionClose_triggered()
{
    emit clear();
    mDatabase->openTempDB();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionAbout_triggered()
{
    mAboutDialog->show();
    mAboutDialog->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mAboutDialog->size(), qApp->desktop()->geometry()));
}

void MainWindow::on_actionAbout_Qt_triggered()
{
    QMessageBox::aboutQt(this, tr("AboutQt"));
}

void MainWindow::on_actionApiKeys_triggered()
{
    mApiKeysDialog->show();
    mApiKeysDialog->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mApiKeysDialog->size(), qApp->desktop()->geometry()));
}

void MainWindow::on_actionPreferences_triggered()
{
    mPreferencesDialog->show();
    mPreferencesDialog->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mPreferencesDialog->size(), qApp->desktop()->geometry()));
}

void MainWindow::on_actionLogViewer_triggered()
{
    mLogViewerDialog->show();
    mLogViewerDialog->setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, mLogViewerDialog->size(), qApp->desktop()->geometry()));
}

void MainWindow::on_actionTwitter_triggered()
{
    QDesktopServices::openUrl(QUrl("https://twitter.com/spider_suite", QUrl::TolerantMode));
}

void MainWindow::on_actionGithub_triggered()
{
    QDesktopServices::openUrl(QUrl("https://github.com/3nock/SpiderSuite", QUrl::TolerantMode));
}

void MainWindow::on_actionWebsite_triggered()
{
    QDesktopServices::openUrl(QUrl("https://SpiderSuite.github.io", QUrl::TolerantMode));
}

void MainWindow::on_actionCheckUpdates_triggered()
{
    mUpdateChecker->checkForUpdates();
}

void MainWindow::on_actionDonate_triggered()
{
    QDesktopServices::openUrl(QUrl("https://spidersuite.github.io/sponsor/", QUrl::TolerantMode));
}

void MainWindow::on_actionNewTemplate_triggered()
{
    this->createNewTemplate();
}
