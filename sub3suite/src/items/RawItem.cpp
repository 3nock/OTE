#include "RawItem.h"


s3s_struct::RAW raw_to_struct(s3s_item::RAW *item){
    s3s_struct::RAW raw;
    raw.raw = item->text();
    raw.result = item_to_json(item);
    return raw;
}

QJsonObject raw_to_json(s3s_item::RAW *item){
    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject raw;
    raw.insert("raw", item->text());
    raw.insert("result", QString::fromUtf8(item_to_json(item)));
    raw.insert("item_info", item_info);
    return raw;
}

void json_to_raw(const QJsonObject &raw, s3s_item::RAW *item){
    item->setText(raw.value("raw").toString());
    item->setValues(raw.value("result").toString().toUtf8());

    QJsonObject item_info = raw.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}

QByteArray item_to_json(QStandardItem *item){
    QJsonObject json;

    if(item->whatsThis() == JSON_OBJECT)
        json.insert(item->text(), setObj(item));

    if(item->whatsThis() == JSON_ARRAY)
        json.insert(item->text(), setArr(item));

    QJsonDocument document;
    document.setObject(json);
    return document.toJson();
}

QJsonObject setObj(QStandardItem *item){
    QJsonObject obj;
    for(int i = 0; i < item->rowCount(); i++)
    {
        QStandardItem *child_item = item->child(i, 0);

        if(child_item->whatsThis() == JSON_OBJECT){
            obj.insert(child_item->text(), setObj(child_item));
            continue;
        }
        if(child_item->whatsThis() == JSON_ARRAY){
            obj.insert(child_item->text(), setArr(child_item));
            continue;
        }
        obj.insert(child_item->text(), item->child(i, 1)->text());
    }
    return obj;
}

QJsonArray setArr(QStandardItem *item){
    QJsonArray arr;
    for(int i = 0; i < item->rowCount(); i++)
    {
        QStandardItem *child_item = item->child(i, 0);

        if(child_item->whatsThis() == JSON_OBJECT){
            arr.append(setObj(child_item));
            continue;
        }
        if(child_item->whatsThis() == JSON_ARRAY){
            arr.append(setArr(child_item));
            continue;
        }
        arr.append(item->child(i, 1)->text());
    }
    return arr;
}
