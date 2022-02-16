#include "RawModel.h"


s3s_struct::RAW raw_to_struct(s3s_item::RAW *item){
    s3s_struct::RAW raw;
    raw.module = item->module;
    raw.target = item->target;
    raw.results = item->json;
    return raw;
}

QJsonObject raw_to_json(s3s_item::RAW *item){
    QJsonObject raw;
    raw.insert("raw", item->text());
    raw.insert("target", item->target);
    raw.insert("module", item->module);
    raw.insert("json", QString::fromUtf8(item->json));
    return raw;
}

void json_to_raw(const QJsonObject &raw, s3s_item::RAW *item){
    item->setText(raw.value("raw").toString());
    item->target = raw.value("target").toString();
    item->module = raw.value("module").toString();
    item->json = raw.value("json").toString().toUtf8();
    item->setValues(item->json);
}
