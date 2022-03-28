#include "EmailItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::Email email_to_struct(s3s_item::Email *item){
    s3s_struct::Email email;

    email.email = item->text();
    email.domain = item->domain->text();
    if(item->free->text() == "true")
        email.free = true;
    else
        email.free = false;

    if(item->hostExists->text() == "true")
        email.hostExists = true;
    else
        email.hostExists = false;

    if(item->disposable->text() == "true")
        email.disposable = true;
    else
        email.disposable = false;

    if(item->deliverable->text() == "true")
        email.deliverable = true;
    else
        email.deliverable = false;

    return email;
}

QJsonObject email_to_json(s3s_item::Email *item){
    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject email;
    email.insert("email", item->text());
    email.insert("domain", item->domain->text());
    email.insert("free", item->free->text());
    email.insert("hostExists", item->hostExists->text());
    email.insert("disposable", item->disposable->text());
    email.insert("deliverable", item->deliverable->text());
    email.insert("item_info", item_info);

    return email;
}

void json_to_email(const QJsonObject &email, s3s_item::Email *item){
    item->setText(email.value("email").toString());
    item->domain->setText(email.value("domain").toString());
    item->free->setText(email.value("free").toString());
    item->hostExists->setText(email.value("hostExists").toString());
    item->disposable->setText(email.value("disposable").toString());
    item->deliverable->setText(email.value("deliverable").toString());

    QJsonObject item_info = email.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
