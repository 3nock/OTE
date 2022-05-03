#include "SSLItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


QJsonObject ssl_to_json(s3s_item::SSL *item){
    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject ssl;

    ssl.insert("ssl", item->text());

    ssl.insert("info_verison", item->info_verison->text());
    ssl.insert("info_serialNumber", item->info_serialNumber->text());
    ssl.insert("info_signatureAlgorithm", item->info_signatureAlgorithm->text());

    /* fingerprint */
    ssl.insert("fingerprint_md5", item->fingerprint_md5->text());
    ssl.insert("fingerprint_sha1", item->fingerprint_sha1->text());
    ssl.insert("fingerprint_sha256", item->fingerprint_sha256->text());

    /* validity */
    ssl.insert("validity_notBefore", item->validity_notBefore->text());
    ssl.insert("validity_notAfter", item->validity_notAfter->text());

    /* issuer Info */
    ssl.insert("issuer_commonName", item->issuer_commonName->text());
    ssl.insert("issuer_organizationName", item->issuer_organizationName->text());
    ssl.insert("issuer_countryName", item->issuer_countryName->text());

    /* subject info */
    ssl.insert("subject_commonName", item->subject_commonName->text());
    ssl.insert("subject_countryName", item->subject_countryName->text());
    ssl.insert("subject_localityName", item->subject_localityName->text());
    ssl.insert("subject_organizationName", item->subject_organizationName->text());
    ssl.insert("subject_stateOrProvinceName", item->subject_stateOrProvinceName->text());
    ssl.insert("subject_email", item->subject_email->text());
    ssl.insert("key_type", item->key_type->text());
    ssl.insert("key_algorithm", item->key_algorithm->text());

    /* alternative names */
    QJsonArray alt_names;
    for(int i = 0; i < item->subjectAltNames->rowCount(); i++)
        alt_names.append(item->subjectAltNames->child(i, 0)->text());
    ssl.insert("alt_names", alt_names);

    ssl.insert("item_info", item_info);

    return ssl;
}

void json_to_ssl(const QJsonObject &ssl, s3s_item::SSL *item){
    item->setText(ssl.value("ssl").toString());

    /* info */
    item->info_verison->setText(ssl.value("info_verison").toString());
    item->info_serialNumber->setText(ssl.value("info_serialNumber").toString());
    item->info_signatureAlgorithm->setText(ssl.value("info_signatureAlgorithm").toString());

    /* fingerprint */
    item->fingerprint_md5->setText(ssl.value("fingerprint_md5").toString());
    item->fingerprint_sha1->setText(ssl.value("fingerprint_sha1").toString());
    item->fingerprint_sha256->setText(ssl.value("fingerprint_sha256").toString());

    /* validity */
    item->validity_notBefore->setText(ssl.value("validity_notBefore").toString());
    item->validity_notAfter->setText(ssl.value("validity_notAfter").toString());

    /* issuer Info */
    item->issuer_commonName->setText(ssl.value("issuer_commonName").toString());
    item->issuer_organizationName->setText(ssl.value("issuer_organizationName").toString());
    item->issuer_countryName->setText(ssl.value("issuer_countryName").toString());

    /* subject info */
    item->subject_commonName->setText(ssl.value("subject_commonName").toString());
    item->subject_countryName->setText(ssl.value("subject_countryName").toString());
    item->subject_localityName->setText(ssl.value("subject_localityName").toString());
    item->subject_organizationName->setText(ssl.value("subject_organizationName").toString());
    item->subject_stateOrProvinceName->setText(ssl.value("subject_stateOrProvinceName").toString());
    item->subject_email->setText(ssl.value("subject_email").toString());
    item->key_type->setText(ssl.value("key_type").toString());
    item->key_algorithm->setText(ssl.value("key_algorithm").toString());

    /* alternative names */
    foreach(const QJsonValue &value, ssl.value("alt_names").toArray())
        item->subjectAltNames->appendRow(new QStandardItem(value.toString()));

    QJsonObject item_info = ssl.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
