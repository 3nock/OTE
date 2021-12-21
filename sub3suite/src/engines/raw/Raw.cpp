#include "Raw.h"
#include "ui_Raw.h"

#include <QMessageBox>
#include <QTextCursor>
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/PassiveConfigDialog.h"


Raw::Raw(QWidget *parent, ProjectDataModel *project, Status *status) :
    AbstractEngine(parent, project, status),
    ui(new Ui::Raw),
    m_model(new QStandardItemModel),
    m_targetListModelHostname(new QStringListModel),
    m_targetListModelIp(new QStringListModel),
    m_targetListModelAsn(new QStringListModel),
    m_targetListModelCidr(new QStringListModel),
    m_targetListModelCert(new QStringListModel),
    m_targetListModelEmail(new QStringListModel)
{
    ui->setupUi(this);

    /* ... */
    ui->buttonStop->setDisabled(true);
    ui->progressBar->hide();

    /* placeholder texts */
    ui->lineEditTarget->setPlaceholderText("Enter target...");
    ui->lineEditFind->setPlaceholderText("Find...");
    ui->lineEditTreeFilter->setPlaceholderText("Filter...");

    /* results model */
    m_model->setColumnCount(2);
    m_model->setHorizontalHeaderLabels({"Key", "Value"});
    ui->treeViewResults->setModel(m_model);

    /* ... */
    ui->labelUrl->setTextFormat(Qt::RichText);
    ui->labelApiDoc->setTextFormat(Qt::RichText);
    ui->labelUrl->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelApiDoc->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelUrl->setOpenExternalLinks(true);
    ui->labelApiDoc->setOpenExternalLinks(true);

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                                        << static_cast<int>((this->width() * 0.50)));

    /* initiate all actions for the context menus */
    this->m_initActions();

    /* syntax higlighting... */
    m_jsonSyntaxHighlighter = new JsonSyntaxHighlighter(ui->plainTextEditResults->document());
    m_notesSyntaxHighlighter = new NotesSyntaxHighlighter(ui->plainTextEditNotes->document());
}
Raw::~Raw(){
    delete m_model;
    delete m_targetListModelIp;
    delete m_targetListModelAsn;
    delete m_targetListModelCert;
    delete m_targetListModelCidr;
    delete m_targetListModelEmail;
    delete m_targetListModelHostname;
    delete ui;
}

void Raw::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void Raw::on_buttonStart_clicked(){
    ui->buttonStart->setDisabled(true);
    ui->buttonStop->setEnabled(true);
    this->m_startScan();
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
    if(ui->plainTextEditResults->find(searchTerm, flags)){
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
    /* set cursor to the begining of the document... */
    QTextCursor currentCursorPosition;
    currentCursorPosition.setPosition(QTextCursor::Start);
    ui->plainTextEditResults->setTextCursor(currentCursorPosition);

    /* get option flags... */
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
