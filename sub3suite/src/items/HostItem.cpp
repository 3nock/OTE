#include "HostItem.h"

#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::HOST host_to_struct(s3s_item::HOST *item){
    s3s_struct::HOST host;

    host.host = item->text();
    host.ipv4 = item->ipv4->text();
    host.ipv6 = item->ipv6->text();
    foreach(const quint16 &port, item->open_ports)
        host.ports.append(port);

    return host;
}

QJsonObject host_to_json(s3s_item::HOST *item){
    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject host;
    host.insert("host", item->text());
    host.insert("ipv4", item->ipv4->text());
    host.insert("ipv6", item->ipv6->text());
    host.insert("ports", item->ports->text());
    host.insert("item_info", item_info);

    return host;
}

void json_to_host(const QJsonObject &host, s3s_item::HOST *item){
    item->setText(host.value("host").toString());
    item->ipv4->setText(host.value("ipv4").toString());
    item->ipv6->setText(host.value("ipv6").toString());
    item->ports->setText(host.value("ports").toString());

    foreach(const QString &port, item->ports->text().split(","))
        item->open_ports.insert(port.toUShort());

    QJsonObject item_info = host.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
