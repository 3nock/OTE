#ifndef RAWITEM_H
#define RAWITEM_H

#include <QStandardItem>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>

#define JSON_ARRAY "arr"
#define JSON_OBJECT "obj"


namespace s3s_struct {
struct RAW {
    QString target;
    QString module;
    QString query_option;
    QByteArray results;
};
}

namespace s3s_item {
class RAW: public QStandardItem {
public:
    RAW(): QStandardItem()
    {
        this->setIcon(QIcon(":/img/res/icons/folder.png"));
    }
    ~RAW()
    {
    }

public:
    QString module;
    QString target;
    QString query_option;
    QByteArray json;

    /* summary */
    QString last_modified;
    QString comment;

    void setValues(const s3s_struct::RAW &raw){
        this->setText(QString("%1 [%2] [%3]").arg(raw.module).arg(raw.query_option).arg(raw.target));
        json = raw.results;
        module = raw.module;
        query_option = raw.query_option;
        target = raw.target;

        QJsonDocument document = QJsonDocument::fromJson(json);
        if(document.isArray())
            this->setArray(document.array(), this);
        if(document.isObject())
            this->setObject(document.object(), this);

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }

    void setValues(const QByteArray &json){
        QJsonDocument document = QJsonDocument::fromJson(json);
        if(document.isArray())
            this->setArray(document.array(), this);
        if(document.isObject())
            this->setObject(document.object(), this);

        /* last modified */
        last_modified = QDate::currentDate().toString();
    }

    void setObject(const QJsonObject &object, QStandardItem *item){
        QStringList keys = object.keys();
        if(item != this){
            item->setWhatsThis(JSON_OBJECT);
            item->setIcon(QIcon(":/img/res/icons/folder2.png"));
        }
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
        if(item != this){
            item->setWhatsThis(JSON_ARRAY);
            item->setIcon(QIcon(":/img/res/icons/folder2.png"));
        }
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


QJsonObject setObj(QStandardItem *item);

QJsonArray setArr(QStandardItem *item);

QByteArray item_to_json(QStandardItem*);

#endif // RAWITEM_H
