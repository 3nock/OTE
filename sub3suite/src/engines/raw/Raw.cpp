#include "Raw.h"
#include "ui_Raw.h"
//...
#include <QMessageBox>
#include <QTextCursor>

#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/PassiveConfigDialog.h"

/*
 * later on use custom icons to indicate this is a string, array or object instead
 * of numbers in the jsonarray..
 */

/*
 * save a list of QStandardItems b4 treeArray & treeObject for later on analysis...
 */
Raw::Raw(QWidget *parent, ResultsModel *resultsModel, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, resultsModel, project, status),
    ui(new Ui::Raw),
    m_model(new QStandardItemModel)
{
    ui->setupUi(this);
    ///
    /// ...
    ///
    ui->buttonStop->setDisabled(true);
    ///
    /// ...
    ///
    ui->lineEditTarget->setPlaceholderText("Enter target...");
    ui->lineEditFind->setPlaceholderText("Find...");
    ui->lineEditTreeFilter->setPlaceholderText("Filter...");
    ///
    ///...
    ///
    m_model->setColumnCount(2);
    m_model->setHorizontalHeaderLabels({"Key", "Value"});
    ui->treeView->setModel(m_model);
    ///
    /// ...
    ///
    ui->labelUrl->setTextFormat(Qt::RichText);
    ui->labelApiDoc->setTextFormat(Qt::RichText);
    ui->labelUrl->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelApiDoc->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelUrl->setOpenExternalLinks(true);
    ui->labelApiDoc->setOpenExternalLinks(true);

    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));
    ///
    /// syntax higlighting...
    ///
    m_jsonSyntaxHighlighter = new JsonSyntaxHighlighter(ui->plainTextEdit->document());
    m_notesSyntaxHighlighter = new NotesSyntaxHighlighter(ui->plainTextEditNotes->document());
}
Raw::~Raw(){
    delete m_model;
    delete ui;
}

void Raw::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void Raw::on_buttonStart_clicked(){
    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);
    this->startScan();
}

void Raw::on_buttonNext_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked()){
        flags |= QTextDocument::FindCaseSensitively;
    }

    this->find(ui->lineEditFind->text(), flags);
}

void Raw::on_buttonPrev_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked()){
        flags |= QTextDocument::FindCaseSensitively;
    }
    flags |= QTextDocument::FindBackward;

    this->find(ui->lineEditFind->text(), flags);
}

void Raw::find(QString searchTerm, QTextDocument::FindFlags flags){
    if(ui->plainTextEdit->find(searchTerm, flags)){
        ui->lineEditFind->setStyleSheet("color: white");
    }else{
        ui->lineEditFind->setStyleSheet("color: rgb(255, 86, 80);");
        return;
    }
}

void Raw::m_infoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText(logTime+log);
}

void Raw::m_errorLog(QString log){
    QString fontedLog("<font color=\"red\">"+log+"</font>");
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendHtml(logTime+fontedLog);
}

void Raw::on_lineEditFind_textEdited(const QString &searchTerm){
    ///
    /// set cursor to the begining of the document...
    ///
    QTextCursor currentCursorPosition;
    currentCursorPosition.setPosition(QTextCursor::Start);
    ui->plainTextEdit->setTextCursor(currentCursorPosition);
    ///
    /// get option flags...
    ///
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked()){
        flags |= QTextDocument::FindCaseSensitively;
    }

    this->find(searchTerm, flags);
}

void Raw::on_comboBoxOptions_currentIndexChanged(const QString &arg1){
    ui->lineEditTarget->clear();
    if(arg1.length() > 0){
        ui->lineEditTarget->setPlaceholderText(m_optionSet.value(arg1).at(0));
        ui->textEditOptionSummary->setText(m_optionSet.value(arg1).at(1));
    }
}

void Raw::on_buttoApiKeys_clicked(){
    ApiKeysDialog *apiKeys = new ApiKeysDialog(this);
    apiKeys->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeys->show();
}

void Raw::on_buttonConfig_clicked(){
    PassiveConfigDialog *scanConfig = new PassiveConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}
