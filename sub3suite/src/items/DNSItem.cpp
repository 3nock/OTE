#include "DNSItem.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


s3s_struct::DNS dns_to_struct(s3s_item::DNS *item){
    s3s_struct::DNS dns;

    dns.dns = item->text();

    for(int i = 0; i < item->A->rowCount(); i++)
        dns.A.insert(item->A->child(i, 1)->text());
    for(int i = 0; i < item->AAAA->rowCount(); i++)
        dns.AAAA.insert(item->AAAA->child(i, 1)->text());
    for(int i = 0; i < item->CNAME->rowCount(); i++)
        dns.CNAME.insert(item->CNAME->child(i, 1)->text());
    for(int i = 0; i < item->NS->rowCount(); i++)
        dns.NS.insert(item->NS->child(i, 1)->text());
    for(int i = 0; i < item->MX->rowCount(); i++)
        dns.MX.insert(item->MX->child(i, 1)->text());
    for(int i = 0; i < item->TXT->rowCount(); i++)
        dns.TXT.insert(item->TXT->child(i, 1)->text());
    for(int i = 0; i < item->SRV->rowCount(); i++)
        dns.SRV.insert({item->SRV->child(i, 0)->text(),
                        item->SRV->child(i, 1)->text(),
                        item->SRV->child(i, 2)->text()});
    return dns;
}

QJsonObject dns_to_json(s3s_item::DNS *item){
    QJsonArray A;
    QJsonArray AAAA;
    QJsonArray CNAME;
    QJsonArray NS;
    QJsonArray MX;
    QJsonArray TXT;
    QJsonArray SRV;

    for(int i = 0; i < item->A->rowCount(); i++)
        A.append(item->A->child(i, 1)->text());
    for(int i = 0; i < item->AAAA->rowCount(); i++)
        AAAA.append(item->AAAA->child(i, 1)->text());
    for(int i = 0; i < item->CNAME->rowCount(); i++)
        CNAME.append(item->CNAME->child(i, 1)->text());
    for(int i = 0; i < item->NS->rowCount(); i++)
        NS.append(item->NS->child(i, 1)->text());
    for(int i = 0; i < item->MX->rowCount(); i++)
        MX.append(item->MX->child(i, 1)->text());
    for(int i = 0; i < item->TXT->rowCount(); i++)
        TXT.append(item->TXT->child(i, 1)->text());
    for(int i = 0; i < item->SRV->rowCount(); i++){
        QJsonArray srv;
        srv.append(item->SRV->child(i, 0)->text());
        srv.append(item->SRV->child(i, 1)->text());
        srv.append(item->SRV->child(i, 2)->text());
        SRV.append(srv);
    }

    QJsonObject item_info;
    item_info.insert("last_modified", item->last_modified);
    item_info.insert("comment", item->comment);

    QJsonObject dns;
    dns.insert("dns", item->text());
    dns.insert("A", A);
    dns.insert("AAAA", AAAA);
    dns.insert("CNAME", CNAME);
    dns.insert("NS", NS);
    dns.insert("MX", MX);
    dns.insert("TXT", TXT);
    dns.insert("SRV", SRV);
    dns.insert("item_info", item_info);

    return dns;
}

void json_to_dns(const QJsonObject &dns, s3s_item::DNS *item){
    item->setText(dns.value("dns").toString());

    int count = 0;
    foreach(const QJsonValue &A, dns.value("A").toArray()){
        item->A->appendRow({new QStandardItem(QString::number(count)),
                            new QStandardItem(A.toString())});
        count++;
    }

    count = 0;
    foreach(const QJsonValue &AAAA, dns.value("AAAA").toArray()){
        item->AAAA->appendRow({new QStandardItem(QString::number(count)),
                            new QStandardItem(AAAA.toString())});
    }

    count = 0;
    foreach(const QJsonValue &CNAME, dns.value("CNAME").toArray()){
        item->CNAME->appendRow({new QStandardItem(QString::number(count)),
                            new QStandardItem(CNAME.toString())});
        count++;
    }

    count = 0;
    foreach(const QJsonValue &NS, dns.value("NS").toArray()){
        item->NS->appendRow({new QStandardItem(QString::number(count)),
                            new QStandardItem(NS.toString())});
        count++;
    }

    count = 0;
    foreach(const QJsonValue &MX, dns.value("MX").toArray()){
        item->MX->appendRow({new QStandardItem(QString::number(count)),
                            new QStandardItem(MX.toString())});
        count++;
    }

    count = 0;
    foreach(const QJsonValue &TXT, dns.value("TXT").toArray()){
        item->TXT->appendRow({new QStandardItem(QString::number(count)),
                            new QStandardItem(TXT.toString())});
        count++;
    }

    foreach(const QJsonValue &SRV, dns.value("SRV").toArray()){
        QJsonArray srv = SRV.toArray();
        item->SRV->appendRow({new QStandardItem(srv.at(0).toString()),
                              new QStandardItem(srv.at(1).toString()),
                              new QStandardItem(srv.at(2).toString())});
    }

    QJsonObject item_info = dns.value("item_info").toObject();
    item->comment = item_info["comment"].toString();
    item->last_modified = item_info["last_modified"].toString();
}
