#include "MXItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::MX mx_to_struct(s3s_item::MX *item){
    s3s_struct::MX mx;

    mx.mx = item->text();

    /* ip */
    for(int i = 0; i < item->ip->rowCount(); i++)
        mx.ip.insert(item->ip->child(i, 0)->text());

    /* domains */
    for(int i = 0; i < item->domains->rowCount(); i++)
        mx.domains.insert(item->domains->child(i, 0)->text());

    return mx;
}

QJsonObject mx_to_json(s3s_item::MX *item){
    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject mx;
    mx.insert("mx", item->text());

    /* ip */
    QJsonArray ip;
    for(int i = 0; i < item->ip->rowCount(); i++)
        ip.append(item->ip->child(i, 0)->text());
    mx.insert("ip", ip);

    /* domains */
    QJsonArray domains;
    for(int i = 0; i < item->domains->rowCount(); i++)
        domains.append(item->domains->child(i, 0)->text());
    mx.insert("domains", domains);

    mx.insert("item_info", item_info);

    return mx;
}

void json_to_mx(const QJsonObject &mx, s3s_item::MX *item){
    item->setText(mx.value("mx").toString());

    /* ip */
    foreach(const QJsonValue &value, mx.value("ip").toArray())
        item->ip->appendRow(new QStandardItem(value.toString()));

    /* domains */
    foreach(const QJsonValue &value, mx.value("domains").toArray())
        item->domains->appendRow(new QStandardItem(value.toString()));

    QJsonObject item_info = mx.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
