#ifndef RAWMODEL_H
#define RAWMODEL_H

#include <QStandardItem>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


namespace s3s_struct {
struct RAW {
    QString target;
    QString module;
    QByteArray results;
};
}

namespace s3s_item {
class RAW: public QStandardItem {
public:
    RAW(): QStandardItem()
    {
        this->setForeground(Qt::white);
        this->setIcon(QIcon(":/img/res/icons/folder.png"));
    }
    ~RAW()
    {
    }

public:
    QString module;
    QString target;
    QByteArray json;

    void setValues(const s3s_struct::RAW &raw){
        this->setText(QString("%1(%2)").arg(raw.module).arg(raw.target));
        json = raw.results;
        module = raw.module;
        target = raw.target;

        QJsonDocument document = QJsonDocument::fromJson(json);
        if(document.isArray())
            this->setArray(document.array(), this);
        if(document.isObject())
            this->setObject(document.object(), this);
    }

    void setValues(const QByteArray &json){
        QJsonDocument document = QJsonDocument::fromJson(json);
        if(document.isArray())
            this->setArray(document.array(), this);
        if(document.isObject())
            this->setObject(document.object(), this);
    }

    void setObject(const QJsonObject &object, QStandardItem *item){
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
                this->setArray(value.toArray(), objectItem);
            }
            if(value.isObject()){
                item->appendRow(objectItem);
                this->setObject(value.toObject(), objectItem);
            }
        }
    }

    void setArray(const QJsonArray &array, QStandardItem *item){
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
                this->setArray(value.toArray(), arrayItem);
            }
            if(value.isObject()){
                item->appendRow(arrayItem);
                this->setObject(value.toObject(), arrayItem);
            }
            count++;
        }
    }

};
}

s3s_struct::RAW raw_to_struct(s3s_item::RAW*);

QJsonObject raw_to_json(s3s_item::RAW*);

void json_to_raw(const QJsonObject&, s3s_item::RAW*);

#endif // RAWMODEL_H
