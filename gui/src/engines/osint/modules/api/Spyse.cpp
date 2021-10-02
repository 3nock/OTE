#include "Spyse.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

/*
    Request Email
    curl -X GET "https://api.spyse.com/v4/data/email/example%40gmail.com" \
        -H "accept: application/json" \
        -H "Authorization: Bearer 3f03ab4b-ab74-46da-a81b-79cde32a3ac0" \
        -H "Content-Type: application/json"

    Response
    {
        data:{
            items:[
                {
                    email:"string"
                    sources:[{...}]
                    updated_at:"2021-05-18T11:47:35.919Z"
                }
                  ]
             }
    }

    Request IpV4
    curl -X GET "https://api.spyse.com/v4/data/ip/8.8.8.8" \
        -H "accept: application/json" \
        -H "Authorization: Bearer 3f03ab4b-ab74-46da-a81b-79cde32a3ac0" \
        -H "Content-Type: application/json"

    Response
    {
        data:{
            items:[{
                abuses:{
                    reports_num:0
                    score:0
                }
                cidr:"string"
                cve_list:[{}]
                geo_info:{
                    city_name:"string"
                    country:"string"
                    country_iso_code:"string"
                    location:{}
                }
                ip:"string"
                isp_info:{
                    as_num:0
                    as_org:"string"
                    isp:"string"
                    updated_at:"2021-05-31T12:48:53.242Z"
                }
                ports:[{}]
                ptr_record:{
                    updated_at:"2021-05-31T12:48:53.244Z"
                    value:"string"
                }
                security_score:{
                    score:0
                }
                technologies:[{}]
                updated_at:"2021-05-31T12:48:53.244Z"
                }
            ]
        }
    }

    Request Bulk-ip
    curl -X POST "https://api.spyse.com/v4/data/bulk-search/ip" \
        -H "accept: application/json" \
        -H "Authorization: Bearer 3f03ab4b-ab74-46da-a81b-79cde32a3ac0" \
        -H "Content-Type: application/json" \
        -d "{\"ip_list\":[\"8.8.8.8\",\"8.8.4.4\"]}"

    Response
        {
        data:{
            limit:0
            offset:0
            total_items:0
            items:[
                {
                abuses:{
                    reports_num:0
                    score:0
                }
                cidr:"string"
                cve_list:[{}]
                geo_info:{
                    city_name:"string"
                    country:"string"
                    country_iso_code:"string"
                    location:{}
                }
                ip:"string"
                isp_info:{
                    as_num:0
                    as_org:"string"
                    isp:"string"
                    updated_at:"2021-05-31T13:08:22.606Z"
                }
                ports:[{}]
                ptr_record:{
                    updated_at:"2021-05-31T13:08:22.606Z"
                    value:"string"
                }
                security_score:{
                    score:0
                }
                technologies:[{}]
                updated_at:"2021-05-31T13:08:22.606Z"
                }
            ]
        }
    }

    Request ASN
    curl -X GET "https://api.spyse.com/v4/data/as/20000" \
        -H "accept: application/json" \
        -H "Authorization: Bearer 3f03ab4b-ab74-46da-a81b-79cde32a3ac0" \
        -H "Content-Type: application/json"

    Response
    {
        data:{
            items:[
                {
                as_org:"string"
                asn:0
                ipv4_prefixes:[{}]
                ipv6_prefixes:[{}]
                }
            ]
        }
    }

    Request Domain
    curl -X GET "https://api.spyse.com/v4/data/domain/twitter.com" \
        -H "accept: application/json" \
        -H "Authorization: Bearer 3f03ab4b-ab74-46da-a81b-79cde32a3ac0" \
        -H "Content-Type: application/json"

    Response
    {
        data:{
            items:[
            {
                alexa:{
                    rank:0
                    updated_at:"2021-05-31T12:46:59.834Z"
                }
                cert_summary:{
                    fingerprint_sha256:"string"
                    issuer:{}
                    issuer_dn:"string"
                    subject:{}
                    subject_dn:"string"
                    tls_version:"string"
                    updated_at:"2021-05-31T12:46:59.834Z"
                    validity_end:"2021-05-31T12:46:59.834Z"
                }
                cve_list:[{}]
                dns_records:{
                    A:[]
                    AAAA:[]
                    CAA:[]
                    CNAME:[]
                    MX:[]
                    NS:[]
                    SOA:{}
                    SPF:[]
                    TXT:[]
                    updated_at:"2021-05-31T12:46:59.834Z"
                }
                hosts_enrichment:[{}]
                http_extract:{
                    cookies:[]
                    description:"string"
                    emails:[]
                    extracted_at:"2021-05-31T12:46:59.835Z"
                    favicon_sha256:"string"
                    final_redirect_url:{}
                    http_headers:[]
                    http_status_code:0
                    links:[]
                    meta_tags:[]
                    robots_txt:"string"
                    scripts:[]
                    styles:[]
                    title:"string"
                }
                is_CNAME:true
                is_MX:true
                is_NS:true
                is_PTR:true
                is_subdomain:true
                name:"string"
                name_without_suffix:"string"
                organizations:[{}]
                screenshot_url:"string"
                security_score:{
                    score:0
                }
                technologies:[{}]
                trackers:{
                    adsense_id:"string"
                    apple_itunes_app:"string"
                    google_analytics_key:"string"
                    google_play_app:"string"
                    google_site_verification:"string"
                }
                updated_at:"2021-05-31T12:46:59.835Z"
                whois_parsed:{
                    admin:{}
                    registrant:{}
                    registrar:{}
                    tech:{}
                    updated_at:"2021-05-31T12:46:59.835Z"
                }
            }
            ]
        }
    }
*/

Spyse::Spyse(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Spyse::replyFinished);
}
Spyse::~Spyse(){
    delete manager;
}

void Spyse::start(){
    QNetworkRequest request;
    QUrl url("https://api.spyse.com/v3/data/domain/subdomain?limit=100&domain="+target);
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("Authorization", "Bearer 3f03ab4b-ab74-46da-a81b-79cde32a3ac0");
    request.setRawHeader("Content-Type", "application/json");
    request.setUrl(url);
    manager->get(request);
}

void Spyse::replyFinished(QNetworkReply *reply){
    if(reply->error())
    {
        emit errorLog(reply->errorString());
    }
    else
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject dataObject = jsonReply.object()["data"].toObject();
        QJsonArray items = dataObject["items"].toArray();
        foreach(const QJsonValue &value, items)
            emit scanResults(value["name"].toString());

        /*
                      Emails
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject dataObject = jsonReply.object()["data"].toObject();
        QJsonArray items = dataObject["items"].toArray();
        foreach(const QJsonValue &value, items)
            emit scanResults(value["email"].toString());

         */
    }
    reply->deleteLater();
    emit quitThread();
}
