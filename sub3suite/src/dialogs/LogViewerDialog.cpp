/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "LogViewerDialog.h"
#include "ui_LogViewerDialog.h"

#include <QDir>
#include <QDateTime>

#include "src/utils/utils.h"


LogViewerDialog::LogViewerDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::LogViewerDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/log.png"));

    /* loading the current logfile to the editor */
    QString currentFile(QDateTime::currentDateTime().toString("dd-MM-yyyy"));
    QFile logfile(QApplication::applicationDirPath()+"/logs/"+currentFile+".log");

    if(logfile.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->plainTextEdit->appendPlainText(logfile.readAll());
        logfile.close();
    }

    /* loading all available logfiles names */
    QDir logsDir(QApplication::applicationDirPath()+"/logs");
    QStringList logfiles = logsDir.entryList(QStringList()<< "*.log");
    logfiles.removeOne(currentFile+".log");  // remove the current file name...
    ui->comboBoxLogFile->addItems(logfiles); // adding the filenames to the log file comboBox...

    /* syntax higlighting... */
    mSyntaxHighlighter = new SyntaxHighlighter(ui->plainTextEdit->document());
    if(s3s_global::is_dark_theme)
        mSyntaxHighlighter->logs_dark();
    if(s3s_global::is_light_theme)
        mSyntaxHighlighter->Logs_light();
}
LogViewerDialog::~LogViewerDialog(){
    delete mSyntaxHighlighter;
    delete ui;
}

void LogViewerDialog::on_comboBoxLogFile_currentIndexChanged(int index){
    /* get logfile name */
    QString logfileName;
    switch (index) {
    case 0:
        logfileName = QString("%1.log").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy"));
        break;
    default:
        logfileName = ui->comboBoxLogFile->currentText();
        break;
    }

    /* clear editor then load the logfile to the editor */
    ui->plainTextEdit->clear();
    QFile logfile(QApplication::applicationDirPath()+"/logs/"+logfileName);
    if(logfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->plainTextEdit->appendPlainText(logfile.readAll());
        logfile.close();
    }
}
