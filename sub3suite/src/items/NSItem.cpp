#include "NSItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::NS ns_to_struct(s3s_item::NS *item){
    s3s_struct::NS ns;

    /* info */
    ns.info_ns = item->info_ns->text();
    ns.info_ip = item->info_ip->text();

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

    /* info */
    ns.insert("info_ns", item->info_ns->text());
    ns.insert("info_ip", item->info_ip->text());

    /* domains */
    QJsonArray domains;
    for(int i = 0; i < item->domains->rowCount(); i++)
        domains.append(item->domains->child(i, 1)->text());
    ns.insert("domains", domains);

    ns.insert("item_info", item_info);

    return ns;
}

void json_to_ns(const QJsonObject &ns, s3s_item::NS *item){
    item->setText(ns.value("info_ns").toString());

    /* info */
    item->info_ns->setText(ns.value("info_ns").toString());
    item->info_ip->setText(ns.value("info_ip").toString());

    /* domains */
    int count = 0;
    foreach(const QJsonValue &value, ns.value("domains").toArray()){
        item->domains->appendRow({new QStandardItem(QString::number(count)),
                                  new QStandardItem(value.toString())});
        count++;
    }

    QJsonObject item_info = ns.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
