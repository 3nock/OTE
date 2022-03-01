#include "CIDRItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardItem>


s3s_struct::CIDR cidr_to_struct(s3s_item::CIDR *item){
    s3s_struct::CIDR cidr_struct;

    cidr_struct.cidr = item->text();

    cidr_struct.info_prefix = item->info_prefix->text();
    cidr_struct.info_ip = item->info_ip->text();
    cidr_struct.info_cidr = item->info_cidr->text();
    cidr_struct.info_name = item->info_name->text();
    cidr_struct.info_description = item->info_description->text();
    cidr_struct.info_country = item->info_country->text();
    cidr_struct.info_website = item->info_website->text();
    cidr_struct.info_ownerAddress = item->info_ownerAddress->text();

    cidr_struct.rir_name = item->rir_name->text();
    cidr_struct.rir_country = item->rir_country->text();
    cidr_struct.rir_dateallocated = item->rir_dateAllocated->text();

    for(int i = 0; i < item->emailContacts->rowCount(); i++)
        cidr_struct.emailcontacts.insert(item->emailContacts->child(i, 1)->text());

    for(int i = 0; i < item->abuseContacts->rowCount(); i++)
        cidr_struct.abusecontacts.insert(item->abuseContacts->child(i, 1)->text());

    for(int i = 0; i < item->asns->rowCount(); i++)
        cidr_struct.asns.insert(item->asns->child(i, 1)->text());

    return cidr_struct;
}

QJsonObject cidr_to_json(s3s_item::CIDR *item){
    QJsonObject info;
    info.insert("prefix", item->info_prefix->text());
    info.insert("ip", item->info_ip->text());
    info.insert("cidr", item->info_cidr->text());
    info.insert("name", item->info_name->text());
    info.insert("description", item->info_description->text());
    info.insert("country", item->info_country->text());
    info.insert("website", item->info_website->text());
    info.insert("ownerAddress", item->info_ownerAddress->text());

    QJsonObject rir;
    rir.insert("name", item->rir_name->text());
    rir.insert("country", item->rir_country->text());
    rir.insert("dateAllocated", item->rir_dateAllocated->text());

    QJsonArray emailContacts;
    for(int i = 0; i < item->emailContacts->rowCount(); i++)
        emailContacts.append(item->emailContacts->child(i, 1)->text());

    QJsonArray abuseContacts;
    for(int i = 0; i < item->abuseContacts->rowCount(); i++)
        abuseContacts.append(item->abuseContacts->child(i, 1)->text());

    QJsonArray asns;
    for(int i = 0; i < item->asns->rowCount(); i++)
        asns.append(item->asns->child(i, 1)->text());

    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject cidrObj;
    cidrObj.insert("cidr", item->text());
    cidrObj.insert("info", info);
    cidrObj.insert("rir", rir);
    cidrObj.insert("email_contacts", emailContacts);
    cidrObj.insert("abuse_contacts", abuseContacts);
    cidrObj.insert("asns", asns);

    cidrObj.insert("item_info", item_info);

    return cidrObj;
}

void json_to_cidr(const QJsonObject &cidr, s3s_item::CIDR *item){
    item->setText(cidr.value("cidr").toString());

    QJsonObject info = cidr.value("info").toObject();
    item->info_prefix->setText(info.value("prefix").toString());
    item->info_ip->setText(info.value("ip").toString());
    item->info_cidr->setText(info.value("cidr").toString());
    item->info_name->setText(info.value("name").toString());
    item->info_description->setText(info.value("description").toString());
    item->info_country->setText(info.value("country").toString());
    item->info_website->setText(info.value("website").toString());
    item->info_ownerAddress->setText(info.value("ownerAddress").toString());

    QJsonObject rir = cidr.value("rir").toObject();
    item->rir_name->setText(rir.value("name").toString());
    item->rir_country->setText(rir.value("country").toString());
    item->rir_dateAllocated->setText(rir.value("dateAllocated").toString());

    int count = 0;
    QJsonArray emailContacts = cidr.value("email_contacts").toArray();
    foreach(const QJsonValue &value, emailContacts){
        item->emailContacts->appendRow({new QStandardItem(QString::number(count)),
                                            new QStandardItem(value.toString())});
        count++;
    }

    count = 0;
    QJsonArray abuseContacts = cidr.value("abuse_contacts").toArray();
    foreach(const QJsonValue &value, abuseContacts){
        item->abuseContacts->appendRow({new QStandardItem(QString::number(count)),
                                            new QStandardItem(value.toString())});
        count++;
    }

    count = 0;
    QJsonArray asns = cidr.value("asns").toArray();
    foreach(const QJsonValue &value, asns){
        item->asns->appendRow({new QStandardItem(QString::number(count)),
                                            new QStandardItem(value.toString())});
        count++;
    }

    QJsonObject item_info = cidr.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
