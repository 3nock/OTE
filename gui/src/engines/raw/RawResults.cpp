#include "Raw.h"
#include "ui_Raw.h"
//...
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


/*
 *
 *
 */

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
