#include "LogViewerDialog.h"
#include "ui_LogViewerDialog.h"

#include <QDir>
#include <QDateTime>


LogViewerDialog::LogViewerDialog(QWidget *parent): QDialog(parent),ui(new Ui::LogViewerDialog)
{
    ui->setupUi(this);

    /* loading the current logfile to the editor */
    QString currentFile(QDateTime::currentDateTime().toString("dd-MM-yyyy"));
    QFile logfile(QDir::currentPath()+"/logs/"+currentFile+".log");

    if(logfile.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->plainTextEdit->appendPlainText(logfile.readAll());
        logfile.close();
    }

    /* loading all available logfiles names */
    QDir logsDir(QDir::currentPath()+"/logs");
    QStringList logfiles = logsDir.entryList(QStringList()<< "*.log");
    logfiles.removeOne(currentFile+".log");  // remove the current file name...
    ui->comboBoxLogFile->addItems(logfiles); // adding the filenames to the log file comboBox...

    /* syntax higlighting... */
    m_logsSyntaxHighlighter = new LogsSyntaxHighlighter(ui->plainTextEdit->document());
}
LogViewerDialog::~LogViewerDialog(){
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
    QFile logfile(QDir::currentPath()+"/logs/"+logfileName);
    if(logfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->plainTextEdit->appendPlainText(logfile.readAll());
        logfile.close();
    }
}

void LogViewerDialog::on_comboBoxLogType_currentIndexChanged(int index){

}
