#include "MXModel.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::MX mx_to_struct(s3s_item::MX *item){
    s3s_struct::MX mx;

    /* info */
    mx.info_mx = item->info_mx->text();
    mx.info_ip = item->info_ip->text();

    /* domains */
    for(int i = 0; i < item->domains->rowCount(); i++)
        mx.domains.insert(item->domains->child(i, 1)->text());

    return mx;
}

QJsonObject mx_to_json(s3s_item::MX *item){
    QJsonObject mx;

    /* info */
    mx.insert("info_mx", item->info_mx->text());
    mx.insert("info_ip", item->info_ip->text());

    /* domains */
    QJsonArray domains;
    for(int i = 0; i < item->domains->rowCount(); i++)
        domains.append(item->domains->child(i, 1)->text());
    mx.insert("domains", domains);

    return mx;
}

void json_to_mx(const QJsonObject &mx, s3s_item::MX *item){
    item->setText(mx.value("info_mx").toString());

    /* info */
    item->info_mx->setText(mx.value("info_mx").toString());
    item->info_ip->setText(mx.value("info_ip").toString());

    /* domains */
    int count = 0;
    foreach(const QJsonValue &value, mx.value("domains").toArray()){
        item->domains->appendRow({new QStandardItem(QString::number(count)),
                                  new QStandardItem(value.toString())});
        count++;
    }
}
