#include "HostItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::HOST host_to_struct(s3s_item::HOST *item){
    s3s_struct::HOST host;

    host.host = item->text();
    host.ipv4 = item->ipv4->text();
    host.ipv6 = item->ipv6->text();
    host.ports = item->open_ports;

    return host;
}

QJsonObject host_to_json(s3s_item::HOST *item){
    QJsonObject host;

    host.insert("host", item->text());
    host.insert("ipv4", item->ipv4->text());
    host.insert("ipv6", item->ipv6->text());
    host.insert("ports", item->ports->text());

    return host;
}

void json_to_host(const QJsonObject &host, s3s_item::HOST *item){
    item->setText(host.value("host").toString());
    item->ipv4->setText(host.value("ipv4").toString());
    item->ipv6->setText(host.value("ipv6").toString());
    item->ports->setText(host.value("ports").toString());
}
