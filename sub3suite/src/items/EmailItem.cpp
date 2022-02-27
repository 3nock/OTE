#include "EmailItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::Email email_to_struct(s3s_item::Email *item){
    s3s_struct::Email email;

    email.email = item->text();
    email.domain = item->domain->text();
    email.address = item->address->text();
    if(item->free->text() == "true")
        email.free = true;
    else
        email.free = false;

    if(item->disposable->text() == "true")
        email.disposable = true;
    else
        email.disposable = false;

    return email;
}

QJsonObject email_to_json(s3s_item::Email *item){
    QJsonObject email;

    email.insert("email", item->text());
    email.insert("domain", item->domain->text());
    email.insert("address", item->address->text());
    email.insert("free", item->free->text());
    email.insert("disposable", item->disposable->text());

    return email;
}

void json_to_email(const QJsonObject &email, s3s_item::Email *item){
    item->setText(email.value("email").toString());
    item->domain->setText(email.value("domain").toString());
    item->address->setText(email.value("address").toString());
    item->free->setText(email.value("free").toString());
    item->disposable->setText(email.value("disposable").toString());
}
