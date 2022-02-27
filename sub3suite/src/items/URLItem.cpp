#include "URLItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::URL url_to_struct(s3s_item::URL *item){
    s3s_struct::URL url;

    url.url = item->text();
    url.banner = item->banner->text();
    url.content_type = item->content_type->text();
    url.status_code = item->status_code->text().toInt();

    return url;
}

QJsonObject url_to_json(s3s_item::URL *item){
    QJsonObject url;

    url.insert("url", item->text());
    url.insert("banner", item->banner->text());
    url.insert("content_type", item->content_type->text());
    url.insert("status_code", item->status_code->text());

    return url;
}

void json_to_url(const QJsonObject &url, s3s_item::URL *item){
    item->setText(url.value("url").toString());
    item->banner->setText(url.value("banner").toString());
    item->content_type->setText(url.value("content_type").toString());
    item->status_code->setText(url.value("status_code").toString());
}
