/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "RawTool.h"
#include "ui_RawTool.h"

#include <QMessageBox>
#include <QTextCursor>
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/config/RawConfigDialog.h"


RawTool::RawTool(QWidget *parent, ProjectModel *project): AbstractTool(parent, project),
    ui(new Ui::RawTool),
    m_model(new QStandardItemModel),
    m_targetListModel(new QStringListModel),
    m_targetListModel_host(new QStringListModel),
    m_targetListModel_ip(new QStringListModel),
    m_targetListModel_asn(new QStringListModel),
    m_targetListModel_cidr(new QStringListModel),
    m_targetListModel_ssl(new QStringListModel),
    m_targetListModel_email(new QStringListModel),
    m_targetListModel_url(new QStringListModel),
    m_scanConfig(new ScanConfig),
    m_scanArgs(new ScanArgs)
{
    this->initUI();

    /* targets model */
    ui->targets->setListName(tr("Targets"));
    ui->targets->setListModel(m_targetListModel);

    /* results model */
    m_model->setHorizontalHeaderLabels({tr(" Properties"), tr(" Values")});
    proxyModel->setSourceModel(m_model);
    proxyModel->setFilterKeyColumn(0);
    ui->treeViewResults->setModel(proxyModel);
    ui->treeViewResults->setHeaderHidden(false);

    m_scanArgs->config = m_scanConfig;
}
RawTool::~RawTool(){
    delete m_scanConfig;
    delete m_scanArgs;
    delete m_targetListModel_url;
    delete m_targetListModel_host;
    delete m_targetListModel_email;
    delete m_targetListModel_cidr;
    delete m_targetListModel_ssl;
    delete m_targetListModel_asn;
    delete m_targetListModel_ip;
    delete m_targetListModel;
    delete m_model;
    delete ui;
}

void RawTool::initUI(){
    ui->setupUi(this);

    /* disabling & hiding widgets */
    ui->buttonStop->setDisabled(true);
    ui->progressBar->hide();

    /* set properties */
    ui->frame->setProperty("default_frame", true);
    ui->labelResultsCount->setProperty("dark", true);
    ui->labelResultsCountTree->setProperty("dark", true);

    /* placeholder texts */
    ui->lineEditTarget->setPlaceholderText(tr("Enter target..."));
    ui->lineEditFind->setPlaceholderText(tr("Find..."));
    ui->lineEditFind_json->setPlaceholderText("Find...");
    ui->lineEditTreeFilter->setPlaceholderText(tr("Filter..."));
    ui->plainTextEditJson->setPlaceholderText("Double-click on an item (first column) on Tree-tab to view it in json...");

    /* ... */
    ui->labelUrl->setTextFormat(Qt::RichText);
    ui->labelApiDoc->setTextFormat(Qt::RichText);
    ui->labelUrl->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelApiDoc->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->labelUrl->setOpenExternalLinks(true);
    ui->labelApiDoc->setOpenExternalLinks(true);

    /* json syntax higlighting */
    m_resultsHighlighter = new SyntaxHighlighter(ui->plainTextEditResults->document());
    m_jsonHighlighter = new SyntaxHighlighter(ui->plainTextEditJson->document());
    if(s3s_global::is_dark_theme){
        m_resultsHighlighter->json_dark();
        m_jsonHighlighter->json_dark();
    }
    if(s3s_global::is_light_theme){
        m_resultsHighlighter->json_light();
        m_jsonHighlighter->json_light();
    }

    /* equally seperate the widgets... */
    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.50))
                           << static_cast<int>((this->width() * 0.50)));

    ui->targets->hide();
    ui->tabWidgetInput->adjustSize();
}

void RawTool::on_lineEditTarget_returnPressed(){
    this->on_buttonStart_clicked();
}

void RawTool::on_buttonStart_clicked(){
    if(ui->lineEditTarget->placeholderText() != PLACEHOLDERTEXT_NONE){
        if(ui->checkBoxMultipleTargets->isChecked() && ui->targets->getlistModel()->rowCount() == 0){
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter Target For Enumerations!"));
            return;
        }
        if(!ui->checkBoxMultipleTargets->isChecked() && ui->lineEditTarget->text().isEmpty()){
            QMessageBox::warning(this, tr("Error!"), tr("Please Enter Targets For Enumerations!"));
            return;
        }
    }

    /* getting the targets */
    m_scanArgs->targets.clear();
    if(ui->checkBoxMultipleTargets->isChecked()){
        foreach(const QString &target, ui->targets->getlistModel()->stringList())
            m_scanArgs->targets.enqueue(target);
    }else
        m_scanArgs->targets.enqueue(ui->lineEditTarget->text());

    /* starting the scan */
    this->startScan();

    /* after starting all choosen modules */
    if(status->activeScanThreads){
        ui->buttonStart->setDisabled(true);
        ui->buttonStop->setEnabled(true);
        log("------------------ start --------------");
        qInfo() << "[RAW] Scan Started";
    }
    else
        QMessageBox::warning(this, tr("Error!"), tr("Please Choose Engine For Enumeration!"));
}

void RawTool::on_buttonStop_clicked(){
    emit stopScanThread();

    status->isStopped = true;
    status->isPaused = false;
    status->isRunning = false;
    status->isNotActive = false;
}

void RawTool::on_comboBoxOptions_currentIndexChanged(const QString &arg1){
    if(arg1.length() == 0)
        return;

    ui->lineEditTarget->clear();
    QString placehodertxt(m_optionSet.value(arg1).at(0));

    ui->lineEditTarget->setPlaceholderText(placehodertxt);
    ui->textEditOptionSummary->setText(m_optionSet.value(arg1).at(1));

    if(placehodertxt == PLACEHOLDERTEXT_ASN || placehodertxt == PLACEHOLDERTEXT_AS){
        ui->targets->setListModel(m_targetListModel_asn);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_IP || placehodertxt == PLACEHOLDERTEXT_IP4 || placehodertxt == PLACEHOLDERTEXT_IP6){
        ui->targets->setListModel(m_targetListModel_ip);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_DOMAIN || placehodertxt == PLACEHOLDERTEXT_HOSTNAME || placehodertxt == PLACEHOLDERTEXT_NS || placehodertxt == PLACEHOLDERTEXT_MX){
        ui->targets->setListModel(m_targetListModel_host);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_CIDR){
        ui->targets->setListModel(m_targetListModel_cidr);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_SSLCERT){
        ui->targets->setListModel(m_targetListModel_ssl);
        return;
    }
    if(placehodertxt == PLACEHOLDERTEXT_URL){
        ui->targets->setListModel(m_targetListModel_url);
        return;
    }
    ui->targets->setListModel(m_targetListModel);
}

void RawTool::on_buttoApiKeys_clicked(){
    ApiKeysDialog *apiKeys = new ApiKeysDialog(this);
    apiKeys->setAttribute(Qt::WA_DeleteOnClose, true);
    apiKeys->show();
}

void RawTool::on_buttonConfig_clicked(){
    RawConfigDialog *scanConfig = new RawConfigDialog(this);
    scanConfig->setAttribute(Qt::WA_DeleteOnClose, true);
    scanConfig->show();
}

void RawTool::log(const QString &log){
    QString logTime = QDateTime::currentDateTime().toString("hh:mm:ss  ");
    ui->plainTextEditLogs->appendPlainText("\n"+logTime+log+"\n");
}

void RawTool::on_lineEditTreeFilter_textChanged(const QString &filterKeyword){
    proxyModel->setFilterKeyColumn(ui->comboBoxFilter->currentIndex());

    if(ui->checkBoxRegex->isChecked())
        proxyModel->setFilterRegExp(QRegExp(filterKeyword));
    else
        proxyModel->setFilterFixedString(filterKeyword);

    ui->treeViewResults->setModel(proxyModel);
    ui->labelResultsCountTree->setNum(proxyModel->rowCount());
}

void RawTool::on_treeViewResults_doubleClicked(const QModelIndex &index){
    if(index.column())
        return;

    QModelIndex model_index = proxyModel->mapToSource(index);

    if(model_index.parent() == m_model->invisibleRootItem()->index()){
        s3s_item::RAW *item = static_cast<s3s_item::RAW*>(m_model->itemFromIndex(model_index));
        ui->plainTextEditJson->setPlainText(item_to_json(item));
        return;
    }

    if(m_model->itemFromIndex(model_index)->hasChildren())
        ui->plainTextEditJson->setPlainText(item_to_json(m_model->itemFromIndex(model_index)));
}

void RawTool::on_tabWidgetInput_currentChanged(int index){
    if(index)
        ui->targets->show();
    else
        ui->targets->hide();

    ui->tabWidgetInput->adjustSize();
}

///
/// find...
///
void RawTool::find(const QString &searchTerm, QTextDocument::FindFlags flags){
    if(ui->plainTextEditResults->find(searchTerm, flags))
        ui->lineEditFind->setStyleSheet("color: white");
    else
        ui->lineEditFind->setStyleSheet("color: rgb(255, 86, 80);");
}

void RawTool::find_json(const QString &searchTerm, QTextDocument::FindFlags flags){
    if(ui->plainTextEditJson->find(searchTerm, flags))
        ui->lineEditFind_json->setStyleSheet("color: white");
    else
        ui->lineEditFind_json->setStyleSheet("color: rgb(255, 86, 80);");
}

void RawTool::on_buttonNext_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find(ui->lineEditFind->text(), flags);
}

void RawTool::on_buttonPrev_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    flags |= QTextDocument::FindBackward;

    this->find(ui->lineEditFind->text(), flags);
}

void RawTool::on_lineEditFind_textEdited(const QString &searchTerm){
    /* set cursor to the begining of the document... */
    QTextCursor currentCursorPosition;
    currentCursorPosition.setPosition(QTextCursor::Start);
    ui->plainTextEditResults->setTextCursor(currentCursorPosition);

    /* get option flags... */
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find(searchTerm, flags);
}

void RawTool::on_buttonNext_json_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive_json->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find(ui->lineEditFind_json->text(), flags);
}

void RawTool::on_buttonPrev_json_clicked(){
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive_json->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    flags |= QTextDocument::FindBackward;

    this->find(ui->lineEditFind_json->text(), flags);
}

void RawTool::on_lineEditFind_json_textChanged(const QString &searchTerm){
    /* set cursor to the begining of the document... */
    QTextCursor currentCursorPosition;
    currentCursorPosition.setPosition(QTextCursor::Start);
    ui->plainTextEditJson->setTextCursor(currentCursorPosition);

    /* get option flags... */
    QTextDocument::FindFlags flags;
    if(ui->checkBoxCaseSensitive_json->isChecked())
        flags |= QTextDocument::FindCaseSensitively;

    this->find_json(searchTerm, flags);
}
