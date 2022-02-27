#include "WildcardItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::Wildcard wildcard_to_struct(s3s_item::Wildcard *item){
    s3s_struct::Wildcard wildcard;

    wildcard.wildcard = item->text();
    wildcard.ipv4 = item->ipv4->text();
    wildcard.ipv6 = item->ipv6->text();

    return wildcard;
}

QJsonObject wildcard_to_json(s3s_item::Wildcard *item){
    QJsonObject wildcard;

    wildcard.insert("wildcard", item->text());
    wildcard.insert("ipv4", item->ipv4->text());
    wildcard.insert("ipv6", item->ipv6->text());

    return wildcard;
}

void json_to_wildcard(const QJsonObject &wildcard, s3s_item::Wildcard *item){
    item->setText(wildcard.value("wildcard").toString());
    item->ipv4->setText(wildcard.value("ipv4").toString());
    item->ipv6->setText(wildcard.value("ipv6").toString());
}
