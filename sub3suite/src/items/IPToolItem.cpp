#include "IPToolItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::IPTool iptool_to_struct(s3s_item::IPTool *item){
    s3s_struct::IPTool ip;

    ip.ip = item->text();

    foreach(const QString &port, item->set_ports)
        ip.ports.insert(port);

    return ip;
}

QJsonObject iptool_to_json(s3s_item::IPTool *item){
    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject ip;

    ip.insert("ip", item->text());
    /* domains */
    QJsonArray ports;
    foreach(const QString &port, item->set_ports)
        ports.append(port);
    ip.insert("ports", ports);

    ip.insert("item_info", item_info);

    return ip;
}

void json_to_iptool(const QJsonObject &ip, s3s_item::IPTool *item){
    item->setText(ip.value("ip").toString());

    foreach(const QJsonValue &value, ip.value("ports").toArray())
        item->addPort(value.toString());

    QJsonObject item_info = ip.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
