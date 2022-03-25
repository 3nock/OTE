#include "NSItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::NS ns_to_struct(s3s_item::NS *item){
    s3s_struct::NS ns;

    ns.ns = item->text();

    /* ip */
    for(int i = 0; i < item->ip->rowCount(); i++)
        ns.ip.insert(item->ip->child(i, 1)->text());

    /* domains */
    for(int i = 0; i < item->domains->rowCount(); i++)
        ns.domains.insert(item->domains->child(i, 1)->text());

    return ns;
}

QJsonObject ns_to_json(s3s_item::NS *item){
    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject ns;
    ns.insert("ns", item->text());

    /* ip */
    QJsonArray ip;
    for(int i = 0; i < item->ip->rowCount(); i++)
        ip.append(item->ip->child(i, 1)->text());
    ns.insert("ip", ip);

    /* domains */
    QJsonArray domains;
    for(int i = 0; i < item->domains->rowCount(); i++)
        domains.append(item->domains->child(i, 1)->text());
    ns.insert("domains", domains);

    ns.insert("item_info", item_info);

    return ns;
}

void json_to_ns(const QJsonObject &ns, s3s_item::NS *item){
    item->setText(ns.value("ns").toString());

    /* ip */
    int count = 0;
    foreach(const QJsonValue &value, ns.value("ip").toArray()){
        item->ip->appendRow({new QStandardItem(QString::number(count)),
                                  new QStandardItem(value.toString())});
        count++;
    }

    /* domains */
    count = 0;
    foreach(const QJsonValue &value, ns.value("domains").toArray()){
        item->domains->appendRow({new QStandardItem(QString::number(count)),
                                  new QStandardItem(value.toString())});
        count++;
    }

    QJsonObject item_info = ns.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
