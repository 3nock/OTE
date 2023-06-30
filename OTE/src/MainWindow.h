//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>

#include "src/core/Database.h"
#include "src/core/UpdateChecker.h"
#include "src/dialogs/AboutDialog.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/LogViewerDialog.h"
#include "src/dialogs/PreferencesDialog.h"
#include "src/views/ExplorerView.h"
#include "src/views/ExtractorView.h"
#include "src/views/WelcomeView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

signals:
    void clear();
    void newProject();
    void newTemplate(OTE::Template *tmplt);

private slots:
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionClose_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionPreferences_triggered();
    void on_actionTwitter_triggered();
    void on_actionGithub_triggered();
    void on_actionWebsite_triggered();
    void on_actionCheckUpdates_triggered();
    void on_actionDonate_triggered();
    void on_actionLogViewer_triggered();
    void on_actionApiKeys_triggered();
    void on_actionNewTemplate_triggered();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;

    OTE::UpdateChecker *mUpdateChecker;

    // database
    OTE::Database *mDatabase;

    // dialogs
    AboutDialog *mAboutDialog;
    ApiKeysDialog *mApiKeysDialog;
    LogViewerDialog *mLogViewerDialog;
    PreferencesDialog *mPreferencesDialog;

    // views
    ExplorerView *mExplorerView;
    ExtractorView *mExtractorView;
    WelcomeView *mWelcomeView;

    // context menus
    QMenu *mMenuRecents;

    void openProject(const QString &project);

    void createNewTemplate();
};

#endif // MAINWINDOW_H
