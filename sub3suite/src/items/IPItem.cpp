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
    ip.info_host = item->info_host->text();
    ip.info_city = item->info_city->text();
    ip.info_region = item->info_region->text();
    ip.info_countryName = item->info_countryName->text();
    ip.info_countryCode = item->info_countryCode->text();
    ip.info_zip = item->info_zip->text();
    ip.info_geoLocation = item->info_geoLocation->text();
    ip.info_organization = item->info_organization->text();
    ip.info_timezone = item->info_timezone->text();
    ip.info_currency = item->info_currency->text();

    /* asn info */
    ip.asnInfo_asn = item->asnInfo_asn->text();
    ip.asnInfo_name = item->asnInfo_name->text();
    ip.asnInfo_domain = item->asnInfo_domain->text();
    ip.asnInfo_route = item->asnInfo_route->text();
    ip.asnInfo_type = item->asnInfo_type->text();

    /* company info */
    ip.companyInfo_name = item->companyInfo_name->text();
    ip.companyInfo_domain = item->companyInfo_domain->text();
    ip.companyInfo_type = item->companyInfo_type->text();

    /* privacy info */
    ip.privacyInfo_vpn = item->privacyInfo_vpn->text();
    ip.privacyInfo_proxy = item->privacyInfo_proxy->text();
    ip.privacyInfo_tor = item->privacyInfo_tor->text();
    ip.privacyInfo_hosting = item->privacyInfo_hosting->text();
    ip.privacyInfo_relay = item->privacyInfo_relay->text();
    ip.privacyInfo_threatLevel = item->privacyInfo_threatLevel->text();

    /* abuse info */
    ip.abuseInfo_address = item->abuseInfo_address->text();
    ip.abuseInfo_country = item->abuseInfo_country->text();
    ip.abuseInfo_email = item->abuseInfo_email->text();
    ip.abuseInfo_name = item->abuseInfo_name->text();
    ip.abuseInfo_network = item->abuseInfo_network->text();
    ip.abuseInfo_phone = item->abuseInfo_phone->text();

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
    ip.insert("info_ip", item->info_ip->text());
    ip.insert("info_type", item->info_type->text());
    ip.insert("info_host", item->info_host->text());
    ip.insert("info_city", item->info_city->text());
    ip.insert("info_region", item->info_region->text());
    ip.insert("info_countryName", item->info_countryName->text());
    ip.insert("info_countryCode", item->info_countryCode->text());
    ip.insert("info_zip", item->info_zip->text());
    ip.insert("info_geoLocation", item->info_geoLocation->text());
    ip.insert("info_organization", item->info_organization->text());
    ip.insert("info_timezone", item->info_timezone->text());
    ip.insert("info_currency", item->info_currency->text());

    /* asnInfo */
    ip.insert("asnInfo_asn", item->asnInfo_asn->text());
    ip.insert("asnInfo_name", item->asnInfo_name->text());
    ip.insert("asnInfo_domain", item->asnInfo_domain->text());
    ip.insert("asnInfo_route", item->asnInfo_route->text());
    ip.insert("asnInfo_type", item->asnInfo_type->text());

    /* companyInfo */
    ip.insert("companyInfo_name", item->companyInfo_name->text());
    ip.insert("companyInfo_domain", item->companyInfo_domain->text());
    ip.insert("companyInfo_type", item->companyInfo_type->text());

    /* privacyInfo */
    ip.insert("privacyInfo_vpn", item->privacyInfo_vpn->text());
    ip.insert("privacyInfo_proxy", item->privacyInfo_proxy->text());
    ip.insert("privacyInfo_tor", item->privacyInfo_tor->text());
    ip.insert("privacyInfo_hosting", item->privacyInfo_hosting->text());
    ip.insert("privacyInfo_relay", item->privacyInfo_relay->text());
    ip.insert("privacyInfo_threatLevel", item->privacyInfo_threatLevel->text());

    /* abuseInfo */
    ip.insert("abuseInfo_address", item->abuseInfo_address->text());
    ip.insert("abuseInfo_country", item->abuseInfo_country->text());
    ip.insert("abuseInfo_email", item->abuseInfo_email->text());
    ip.insert("abuseInfo_name", item->abuseInfo_name->text());
    ip.insert("abuseInfo_network", item->abuseInfo_network->text());
    ip.insert("abuseInfo_phone", item->abuseInfo_phone->text());

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
    item->info_ip->setText(ip.value("info_ip").toString());
    item->info_type->setText(ip.value("info_type").toString());
    item->info_host->setText(ip.value("info_host").toString());
    item->info_city->setText(ip.value("info_city").toString());
    item->info_region->setText(ip.value("info_region").toString());
    item->info_countryName->setText(ip.value("info_countryName").toString());
    item->info_countryCode->setText(ip.value("info_countryCode").toString());
    item->info_zip->setText(ip.value("info_zip").toString());
    item->info_geoLocation->setText(ip.value("info_geoLocation").toString());
    item->info_organization->setText(ip.value("info_organization").toString());
    item->info_timezone->setText(ip.value("info_timezone").toString());
    item->info_currency->setText(ip.value("info_currency").toString());

    /* asnInfo */
    item->asnInfo_asn->setText(ip.value("asnInfo_asn").toString());
    item->asnInfo_name->setText(ip.value("asnInfo_name").toString());
    item->asnInfo_domain->setText(ip.value("asnInfo_domain").toString());
    item->asnInfo_route->setText(ip.value("asnInfo_route").toString());
    item->asnInfo_type->setText(ip.value("asnInfo_type").toString());

    /* companyInfo */
    item->companyInfo_name->setText(ip.value("companyInfo_name").toString());
    item->companyInfo_domain->setText(ip.value("companyInfo_domain").toString());
    item->companyInfo_type->setText(ip.value("companyInfo_type").toString());

    /* privacyInfo */
    item->privacyInfo_vpn->setText(ip.value("privacyInfo_vpn").toString());
    item->privacyInfo_proxy->setText(ip.value("privacyInfo_proxy").toString());
    item->privacyInfo_tor->setText(ip.value("privacyInfo_tor").toString());
    item->privacyInfo_hosting->setText(ip.value("privacyInfo_hosting").toString());
    item->privacyInfo_relay->setText(ip.value("privacyInfo_relay").toString());
    item->privacyInfo_threatLevel->setText(ip.value("privacyInfo_threatLevel").toString());

    /* abuseInfo */
    item->abuseInfo_address->setText(ip.value("abuseInfo_address").toString());
    item->abuseInfo_country->setText(ip.value("abuseInfo_country").toString());
    item->abuseInfo_email->setText(ip.value("abuseInfo_email").toString());
    item->abuseInfo_name->setText(ip.value("abuseInfo_name").toString());
    item->abuseInfo_network->setText(ip.value("abuseInfo_network").toString());
    item->abuseInfo_phone->setText(ip.value("abuseInfo_phone").toString());

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
