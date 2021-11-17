#include "Raw.h"
#include "ui_Raw.h"
//...
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>
#include <QTextCursor>

/*
 * later on use custom icons to indicate this is a string, array or object instead
 * of numbers in the jsonarray..
 */

/*
 * save a list of QStandardItems b4 treeArray & treeObject for later on analysis...
 */
Raw::Raw(QWidget *parent) : QDialog(parent), ui(new Ui::Raw),
    m_model(new QStandardItemModel),
    m_scanArgs(new ScanArgs)
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
    /// json syntax higlighting...
    ///
    syntaxHighlighter = new JsonSyntaxHighlighter(ui->plainTextEdit->document());
}
Raw::~Raw(){
    delete m_scanArgs;
    delete m_model;
    delete ui;
}

void Raw::onEnumerationComplete(){
    ui->buttonStart->setEnabled(true);
    ui->buttonStop->setDisabled(true);
}

void Raw::onErrorLog(QString log){
    QString fontedLog;
    fontedLog.append("<font color=\"red\">").append(log).append("</font>");
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLogs->appendHtml(logTime.append(fontedLog));
}

void Raw::onInfoLog(QString log){
    QString logTime = QDateTime::currentDateTime().toString("[hh:mm]  ");
    ui->plainTextEditLogs->appendPlainText(logTime.append(log));
}

void Raw::onResults(QByteArray reply){
    QJsonDocument document = QJsonDocument::fromJson(reply);
    setJsonText(document);
    setJsonTree(document);
}

void Raw::onResultsTxt(QByteArray results){
    if(!results.isNull() && !results.isEmpty()){
        ui->plainTextEdit->appendPlainText(results);
        m_resultsCountJson++;
        ui->labelResultsCount->setNum(m_resultsCountJson);
    }
}

void Raw::setJsonText(QJsonDocument &results){
    if(!results.isNull() && !results.isEmpty()){
        ui->plainTextEdit->appendPlainText(results.toJson());
        m_resultsCountJson++;
        ui->labelResultsCount->setNum(m_resultsCountJson);
    }
}

void Raw::setJsonTree(QJsonDocument &results){
    if(results.isNull() || results.isEmpty())
        return;

    QStandardItem *item = new QStandardItem(m_scanArgs->module+"("+m_scanArgs->target+")");

    QFont font("Segoe UI", 9, QFont::Bold);
    item->setFont(font);
    item->setForeground(QColor(220,220,220));

    m_model->invisibleRootItem()->appendRow(item);

    if(results.isArray())
        this->treeArray(results.array(), item);
    if(results.isObject())
        this->treeObject(results.object(), item);
    ///
    /// results Count...
    ///
    m_resultsCountTree++;
    ui->labelResultsCountTree->setNum(m_resultsCountTree);
}

void Raw::treeObject(QJsonObject object, QStandardItem *item){
    QStringList keys = object.keys();
    foreach(const QString &key, keys)
    {
        QJsonValue value = object.value(key);
        if(value.isUndefined())
            continue;

        QStandardItem * objectItem = new QStandardItem(key);

        if(value.isString())
            item->appendRow({objectItem, new QStandardItem(value.toString())});
        if(value.isDouble())
            item->appendRow({objectItem, new QStandardItem(QString::number(value.toDouble()))});
        if(value.isNull())
            item->appendRow({objectItem, new QStandardItem("null")});
        if(value.isBool()){
            if(value.toBool())
                item->appendRow({objectItem, new QStandardItem("true")});
            else
                item->appendRow({objectItem, new QStandardItem("false")});
        }
        if(value.isArray()){
            item->appendRow(objectItem);
            this->treeArray(value.toArray(), objectItem);
        }
        if(value.isObject()){
            item->appendRow(objectItem);
            this->treeObject(value.toObject(), objectItem);
        }
    }
}

void Raw::treeArray(QJsonArray array, QStandardItem *item){
    int count = 0;
    foreach(const QJsonValue &value, array)
    {
        if(value.isUndefined())
            continue;

        QStandardItem *arrayItem = new QStandardItem(QString::number(count));

        if(value.isString())
            item->appendRow({arrayItem, new QStandardItem(value.toString())});
        if(value.isDouble())
            item->appendRow({arrayItem, new QStandardItem(QString::number(value.toDouble()))});
        if(value.isNull())
            item->appendRow({arrayItem, new QStandardItem("null")});
        if(value.isBool()){
            if(value.toBool())
                item->appendRow({arrayItem, new QStandardItem("true")});
            else
                item->appendRow({arrayItem, new QStandardItem("false")});
        }
        if(value.isArray()){
            item->appendRow(arrayItem);
            this->treeArray(value.toArray(), arrayItem);
        }
        if(value.isObject()){
            item->appendRow(arrayItem);
            this->treeObject(value.toObject(), arrayItem);
        }
        count++;
    }
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
