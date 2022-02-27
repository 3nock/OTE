#include "ASNItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QStandardItem>


s3s_struct::ASN asn_to_struct(s3s_item::ASN *item){
    s3s_struct::ASN asn_struct;

    asn_struct.asn = item->text();

    asn_struct.info_asn = item->info_asn->text();
    asn_struct.info_name = item->info_name->text();
    asn_struct.info_description = item->info_description->text();
    asn_struct.info_country = item->info_country->text();
    asn_struct.info_website = item->info_website->text();
    asn_struct.info_ownerAddress = item->info_ownerAddress->text();

    asn_struct.rir_name = item->rir_name->text();
    asn_struct.rir_country = item->rir_country->text();
    asn_struct.rir_dateallocated = item->rir_dateAllocated->text();

    for(int i = 0; i < item->emailContacts->rowCount(); i++)
        asn_struct.emailcontacts.insert(item->emailContacts->child(i, 1)->text());

    for(int i = 0; i < item->abuseContacts->rowCount(); i++)
        asn_struct.abusecontacts.insert(item->abuseContacts->child(i, 1)->text());

    for(int i = 0; i < item->peers->rowCount(); i++)
        asn_struct.peers.insert(item->peers->child(i, 1)->text());

    for(int i = 0; i < item->prefixes->rowCount(); i++)
        asn_struct.prefixes.insert(item->prefixes->child(i, 1)->text());

    return asn_struct;
}

QJsonObject asn_to_json(s3s_item::ASN *item){
    QJsonObject info;
    info.insert("asn", item->info_asn->text());
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

    QJsonArray peers;
    for(int i = 0; i < item->peers->rowCount(); i++)
        peers.append(item->peers->child(i, 1)->text());

    QJsonArray prefixes;
    for(int i = 0; i < item->prefixes->rowCount(); i++)
        prefixes.append(item->prefixes->child(i, 1)->text());

    QJsonObject asnObj;
    asnObj.insert("asn", item->text());
    asnObj.insert("info", info);
    asnObj.insert("rir", rir);
    asnObj.insert("email_contacts", emailContacts);
    asnObj.insert("abuse_contacts", abuseContacts);
    asnObj.insert("peers", peers);
    asnObj.insert("prefixes", prefixes);

    return asnObj;
}

void json_to_asn(const QJsonObject &jsonObj, s3s_item::ASN *item){
    item->setText(jsonObj.value("asn").toString());

    QJsonObject info = jsonObj.value("info").toObject();
    item->info_asn->setText(info.value("asn").toString());
    item->info_name->setText(info.value("name").toString());
    item->info_description->setText(info.value("description").toString());
    item->info_country->setText(info.value("country").toString());
    item->info_website->setText(info.value("website").toString());
    item->info_ownerAddress->setText(info.value("ownerAddress").toString());

    QJsonObject rir = jsonObj.value("rir").toObject();
    item->rir_name->setText(rir.value("name").toString());
    item->rir_country->setText(rir.value("country").toString());
    item->rir_dateAllocated->setText(rir.value("dateAllocated").toString());

    int count = 0;
    QJsonArray emailContacts = jsonObj.value("email_contacts").toArray();
    foreach(const QJsonValue &value, emailContacts){
        item->emailContacts->appendRow({new QStandardItem(QString::number(count)),
                                            new QStandardItem(value.toString())});
        count++;
    }

    count = 0;
    QJsonArray abuseContacts = jsonObj.value("abuse_contacts").toArray();
    foreach(const QJsonValue &value, abuseContacts){
        item->abuseContacts->appendRow({new QStandardItem(QString::number(count)),
                                            new QStandardItem(value.toString())});
        count++;
    }

    count = 0;
    QJsonArray peers = jsonObj.value("peers").toArray();
    foreach(const QJsonValue &value, peers){
        item->peers->appendRow({new QStandardItem(QString::number(count)),
                                            new QStandardItem(value.toString())});
        count++;
    }

    count = 0;
    QJsonArray prefixes = jsonObj.value("prefixes").toArray();
    foreach(const QJsonValue &value, prefixes){
        item->prefixes->appendRow({new QStandardItem(QString::number(count)),
                                            new QStandardItem(value.toString())});
        count++;
    }
}
