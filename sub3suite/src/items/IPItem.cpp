#include "IPItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::IP ip_to_struct(s3s_item::IP *item){
    s3s_struct::IP ip;

    ip.ip = item->text();

    /* ip info */
    ip.info_ip = item->info_ip->text();
    ip.info_type = item->info_type->text();
    ip.info_city = item->info_city->text();
    ip.info_region = item->info_region->text();
    ip.info_countryName = item->info_countryName->text();
    ip.info_countryCode = item->info_countryCode->text();
    ip.info_zip = item->info_zip->text();
    ip.info_geoLocation = item->info_geoLocation->text();
    ip.info_organization = item->info_organization->text();
    ip.info_timezone = item->info_timezone->text();

    /* asn info */
    ip.asnInfo_asn = item->asnInfo_asn->text();
    ip.asnInfo_name = item->asnInfo_name->text();
    ip.asnInfo_route = item->asnInfo_route->text();

    /* company info */
    ip.companyInfo_name = item->companyInfo_name->text();
    ip.companyInfo_domain = item->companyInfo_domain->text();

    /* privacy info */
    ip.privacyInfo_vpn = QVariant(item->privacyInfo_vpn->text()).toBool();
    ip.privacyInfo_proxy = QVariant(item->privacyInfo_proxy->text()).toBool();
    ip.privacyInfo_anonymous = QVariant(item->privacyInfo_anonymous->text()).toBool();
    ip.privacyInfo_tor = QVariant(item->privacyInfo_tor->text()).toBool();
    ip.privacyInfo_attacker = QVariant(item->privacyInfo_attacker->text()).toBool();
    ip.privacyInfo_abuser = QVariant(item->privacyInfo_abuser->text()).toBool();
    ip.privacyInfo_threat = QVariant(item->privacyInfo_threat->text()).toBool();

    /* domains */
    for(int i = 0; i < item->domains->rowCount(); i++)
        ip.domains.insert(item->domains->child(i, 1)->text());

    return ip;
}

QJsonObject ip_to_json(s3s_item::IP *item){
    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject ip;

    /* info */
    ip.insert("ip", item->text());
    ip.insert("info_ip", item->info_ip->text());
    ip.insert("info_type", item->info_type->text());
    ip.insert("info_city", item->info_city->text());
    ip.insert("info_region", item->info_region->text());
    ip.insert("info_countryName", item->info_countryName->text());
    ip.insert("info_countryCode", item->info_countryCode->text());
    ip.insert("info_zip", item->info_zip->text());
    ip.insert("info_geoLocation", item->info_geoLocation->text());
    ip.insert("info_organization", item->info_organization->text());
    ip.insert("info_timezone", item->info_timezone->text());

    /* asnInfo */
    ip.insert("asnInfo_asn", item->asnInfo_asn->text());
    ip.insert("asnInfo_name", item->asnInfo_name->text());
    ip.insert("asnInfo_route", item->asnInfo_route->text());

    /* companyInfo */
    ip.insert("companyInfo_name", item->companyInfo_name->text());
    ip.insert("companyInfo_domain", item->companyInfo_domain->text());

    /* privacyInfo */
    ip.insert("privacyInfo_vpn", item->privacyInfo_vpn->text());
    ip.insert("privacyInfo_proxy", item->privacyInfo_proxy->text());
    ip.insert("privacyInfo_anonymous", item->privacyInfo_anonymous->text());
    ip.insert("privacyInfo_tor", item->privacyInfo_tor->text());
    ip.insert("privacyInfo_attacker", item->privacyInfo_attacker->text());
    ip.insert("privacyInfo_abuser", item->privacyInfo_abuser->text());
    ip.insert("privacyInfo_threat", item->privacyInfo_threat->text());

    /* domains */
    QJsonArray domains;
    for(int i = 0; i < item->domains->rowCount(); i++)
        domains.append(item->domains->child(i, 1)->text());
    ip.insert("domains", domains);

    ip.insert("item_info", item_info);

    return ip;
}

void json_to_ip(const QJsonObject &ip, s3s_item::IP *item){
    /* info */
    item->setText(ip.value("ip").toString());
    item->info_ip->setText(ip.value("info_ip").toString());
    item->info_type->setText(ip.value("info_type").toString());
    item->info_city->setText(ip.value("info_city").toString());
    item->info_region->setText(ip.value("info_region").toString());
    item->info_countryName->setText(ip.value("info_countryName").toString());
    item->info_countryCode->setText(ip.value("info_countryCode").toString());
    item->info_zip->setText(ip.value("info_zip").toString());
    item->info_geoLocation->setText(ip.value("info_geoLocation").toString());
    item->info_organization->setText(ip.value("info_organization").toString());
    item->info_timezone->setText(ip.value("info_timezone").toString());

    /* asnInfo */
    item->asnInfo_asn->setText(ip.value("asnInfo_asn").toString());
    item->asnInfo_name->setText(ip.value("asnInfo_name").toString());
    item->asnInfo_route->setText(ip.value("asnInfo_route").toString());

    /* companyInfo */
    item->companyInfo_name->setText(ip.value("companyInfo_name").toString());
    item->companyInfo_domain->setText(ip.value("companyInfo_domain").toString());

    /* privacyInfo */
    item->privacyInfo_vpn->setText(ip.value("privacyInfo_vpn").toString());
    item->privacyInfo_proxy->setText(ip.value("privacyInfo_proxy").toString());
    item->privacyInfo_anonymous->setText(ip.value("privacyInfo_anonymous").toString());
    item->privacyInfo_tor->setText(ip.value("privacyInfo_tor").toString());
    item->privacyInfo_attacker->setText(ip.value("privacyInfo_attacker").toString());
    item->privacyInfo_abuser->setText(ip.value("privacyInfo_abuser").toString());
    item->privacyInfo_threat->setText(ip.value("privacyInfo_threat").toString());

    /* domains */
    int count = 0;
    foreach(const QJsonValue &value, ip.value("domains").toArray()){
        item->domains->appendRow({new QStandardItem(QString::number(count)),
                                  new QStandardItem(value.toString())});
        count++;
    }

    QJsonObject item_info = ip.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
