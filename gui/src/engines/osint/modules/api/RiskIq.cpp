#include "RiskIq.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

/*
 * Error in Basic Authentication
 */
RiskIq::RiskIq(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &RiskIq::replyFinished);
    ///
    /// getting api-key...
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_name = Config::generalConfig().value("riskiq_username").toString();
    m_key = Config::generalConfig().value("riskiq_key").toString();
    Config::generalConfig().endGroup();
}
RiskIq::~RiskIq(){
    delete manager;
}

void RiskIq::start(){
    QNetworkRequest request;
    QUrl url("https://api.riskiq.net/v0/pdns/name?name="+target+"&type=A");
    request.setUrl(url);
    request.setRawHeader("Accept", "application/json");
    ///
    /// HTTP Basic authentication header value: base64(username:password)
    ///
    QString concatenated = m_name+":"+m_key;
    QByteArray data = concatenated.toLocal8Bit().toBase64();
    /*
    QString headerData = "basic " + data;
    request.setRawHeader("Authorization", headerData.toLocal8Bit());
    */
    request.setRawHeader("Authorization", "Basic "+data);
    manager->get(request);
}

void RiskIq::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject results = jsonReply.object();
        int count = results["recordCount"].toInt();
        if(count){
            QJsonArray records = results["records"].toArray();
            foreach(const QJsonValue &value, records)
            {
                QJsonArray data = value.toObject()["data"].toArray();
                foreach(const QJsonValue &value, data)
                    emit scanResults(value.toString());
            }
        }
        QJsonArray subdomainList = jsonReply.array();
        foreach(const QJsonValue &value, subdomainList)
            emit scanResults(value.toString());
    }
    else{
        emit errorLog(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

/* DNS
 * https://api.riskiq.net/v0/pdns/name?name=riskiq.net&type=A
 * https://api.riskiq.net/v0/pdns/data/ip?ip=8.8.8.8
 * https://api.riskiq.net/v0/pdns/data/raw?hex=00cf
 * https://api.riskiq.net/v0/pdns/data/name?name=riskiq.net
RESPONSE:
{
  "recordCount": 5,
  "records": [
    {
      "count": 5,
      "firstSeen": "2009-10-21T14:23:09.000-0700",
      "lastSeen": "2013-01-04T11:35:05.000-0800",
      "name": "riskiq.net.",
      "data": [
        "70.32.68.247"
      ],
      "rrtype": "A"
    },
    {
      "count": 1,
      "firstSeen": "2013-07-19T08:54:00.000-0700",
      "lastSeen": "2013-07-19T08:54:00.000-0700",
      "name": "riskiq.net.",
      "data": [
        "162.209.9.7"
      ],
      "rrtype": "A"
    },
    {
      "count": 411,
      "firstSeen": "2010-08-29T05:06:00.000-0700",
      "lastSeen": "2013-07-06T15:55:37.000-0700",
      "name": "riskiq.net.",
      "data": [
        "70.32.68.247"
      ],
      "rrtype": "A"
    },
    {
      "count": 301065,
      "firstSeen": "2015-07-23T10:33:27.000-0700",
      "lastSeen": "2016-07-21T06:34:05.000-0700",
      "name": "riskiq.net.",
      "data": [
        "162.209.8.240"
      ],
      "rrtype": "A"
    },
    {
      "count": 1848,
      "firstSeen": "2013-07-19T08:54:00.000-0700",
      "lastSeen": "2015-07-22T06:08:27.000-0700",
      "name": "riskiq.net.",
      "data": [
        "162.209.9.7"
      ],
      "rrtype": "A"
    }
  ]
}

 *
 * WHOIS
 * https://api.riskiq.net/v0/whois/domain?domain=
 * https://api.riskiq.net/v0/whois/address
 * https://api.riskiq.net/v0/whois/email
 * https://api.riskiq.net/v0/whois/name
 * https://api.riskiq.net/v0/whois/nameserver
 * https://api.riskiq.net/v0/whois/org
 * https://api.riskiq.net/v0/whois/phone
RESPONSE:
{
  "results": 1,
  "domains": [
    {
      "domain": "riskiq.com",
      "registrar": "GODADDY.COM, LLC",
      "whoisServer": "whois.godaddy.com",
      "registered": "2006-01-11T16:00:00.000-0800",
      "registryUpdatedAt": "2014-12-08T16:00:00.000-0800",
      "expiresAt": "2017-01-11T16:00:00.000-0800",
      "contactEmail": "domains@riskiq.com",
      "nameServers": [
        "luke.ns.cloudflare.com",
        "serena.ns.cloudflare.com"
      ],
      "registrant": {
        "email": "domains@riskiq.com",
        "name": "Risk IQ",
        "organization": "RiskIQ, Inc.",
        "street": "22 Battery Street\n10th Floor",
        "city": "San Francisco",
        "state": "California",
        "postalCode": "94111",
        "country": "US",
        "telephone": "18884154447"
      },
      "admin": {
        "email": "domains@riskiq.com",
        "name": "Risk IQ",
        "organization": "RiskIQ, Inc.",
        "street": "22 Battery Street\n10th Floor",
        "city": "San Francisco",
        "state": "California",
        "postalCode": "94111",
        "country": "US",
        "telephone": "18884154447"
      },
      "billing": {},
      "tech": {
        "email": "domains@riskiq.com",
        "name": "Risk IQ",
        "organization": "RiskIQ, Inc.",
        "street": "22 Battery Street\n10th Floor",
        "city": "San Francisco",
        "state": "California",
        "postalCode": "94111",
        "country": "US",
        "telephone": "18884154447"
      },
      "zone": {},
      "text": "Whois Server Version 2.0\n\nDomain names in the .com and .net domains can now be registered\nwith many different competing registrars. Go to http://www.internic.net\nfor detailed information.\n\n   Domain Name: RISKIQ.COM\n   Registrar: GODADDY.COM, LLC\n   Sponsoring Registrar IANA ID: 146\n   Whois Server: whois.godaddy.com\n   Referral URL: http://www.godaddy.com\n   Name Server: LUKE.NS.CLOUDFLARE.COM\n   Name Server: SERENA.NS.CLOUDFLARE.COM\n   Status: clientDeleteProhibited https://icann.org/epp#clientDeleteProhibited\n   Status: clientRenewProhibited https://icann.org/epp#clientRenewProhibited\n   Status: clientTransferProhibited https://icann.org/epp#clientTransferProhibited\n   Status: clientUpdateProhibited https://icann.org/epp#clientUpdateProhibited\n   Updated Date: 09-dec-2014\n   Creation Date: 12-jan-2006\n   Expiration Date: 12-jan-2017\n\n>>> Last update of whois database: Wed, 22 Jun 2016 22:24:21 GMT <<<\n\nFor more information on Whois status codes, please visit https://icann.org/epp\n\nNOTICE: The expiration date displayed in this record is the date the\nregistrar's sponsorship of the domain name registration in the registry is\ncurrently set to expire. This date does not necessarily reflect the expiration\ndate of the domain name registrant's agreement with the sponsoring\nregistrar.  Users may consult the sponsoring registrar's Whois database to\nview the registrar's reported date of expiration for this registration.\n\nTERMS OF USE: You are not authorized to access or query our Whois\ndatabase through the use of electronic processes that are high-volume and\nautomated except as reasonably necessary to register domain names or\nmodify existing registrations; the Data in VeriSign Global Registry\nServices' (\"VeriSign\") Whois database is provided by VeriSign for\ninformation purposes only, and to assist persons in obtaining information\nabout or related to a domain name registration record. VeriSign does not\nguarantee its accuracy. By submitting a Whois query, you agree to abide\nby the following terms of use: You agree that you may use this Data only\nfor lawful purposes and that under no circumstances will you use this Data\nto: (1) allow, enable, or otherwise support the transmission of mass\nunsolicited, commercial advertising or solicitations via e-mail, telephone,\nor facsimile; or (2) enable high volume, automated, electronic processes\nthat apply to VeriSign (or its computer systems). The compilation,\nrepackaging, dissemination or other use of this Data is expressly\nprohibited without the prior written consent of VeriSign. You agree not to\nuse electronic processes that are automated and high-volume to access or\nquery the Whois database except as reasonably necessary to register\ndomain names or modify existing registrations. VeriSign reserves the right\nto restrict your access to the Whois database in its sole discretion to ensure\noperational stability.  VeriSign may restrict or terminate your access to the\nWhois database for failure to abide by these terms of use. VeriSign\nreserves the right to modify these terms at any time.\n\nThe Registry database contains ONLY .COM, .NET, .EDU domains and\nRegistrars.\n\nDomain Name: RISKIQ.COM\nRegistry Domain ID: 314061295_DOMAIN_COM-VRSN\nRegistrar WHOIS Server: whois.godaddy.com\nRegistrar URL: http://www.godaddy.com\nUpdate Date: 2014-12-09T00:34:46Z\nCreation Date: 2006-01-12T19:33:26Z\nRegistrar Registration Expiration Date: 2017-01-12T19:33:26Z\nRegistrar: GoDaddy.com, LLC\nRegistrar IANA ID: 146\nRegistrar Abuse Contact Email: abuse@godaddy.com\nRegistrar Abuse Contact Phone: +1.4806242505\nDomain Status: clientTransferProhibited http://www.icann.org/epp#clientTransferProhibited\nDomain Status: clientUpdateProhibited http://www.icann.org/epp#clientUpdateProhibited\nDomain Status: clientRenewProhibited http://www.icann.org/epp#clientRenewProhibited\nDomain Status: clientDeleteProhibited http://www.icann.org/epp#clientDeleteProhibited\nRegistry Registrant ID: Not Available From Registry\nRegistrant Name: Risk IQ\nRegistrant Organization: RiskIQ, Inc.\nRegistrant Street: 22 Battery Street\nRegistrant Street: 10th Floor\nRegistrant City: San Francisco\nRegistrant State/Province: California\nRegistrant Postal Code: 94111\nRegistrant Country: US\nRegistrant Phone: +1.8884154447\nRegistrant Phone Ext: \nRegistrant Fax: \nRegistrant Fax Ext: \nRegistrant Email: domains@riskiq.com\nRegistry Admin ID: Not Available From Registry\nAdmin Name: Risk IQ\nAdmin Organization: RiskIQ, Inc.\nAdmin Street: 22 Battery Street\nAdmin Street: 10th Floor\nAdmin City: San Francisco\nAdmin State/Province: California\nAdmin Postal Code: 94111\nAdmin Country: US\nAdmin Phone: +1.8884154447\nAdmin Phone Ext: \nAdmin Fax: \nAdmin Fax Ext: \nAdmin Email: domains@riskiq.com\nRegistry Tech ID: Not Available From Registry\nTech Name: Risk IQ\nTech Organization: RiskIQ, Inc.\nTech Street: 22 Battery Street\nTech Street: 10th Floor\nTech City: San Francisco\nTech State/Province: California\nTech Postal Code: 94111\nTech Country: US\nTech Phone: +1.8884154447\nTech Phone Ext: \nTech Fax: \nTech Fax Ext: \nTech Email: domains@riskiq.com\nName Server: LUKE.NS.CLOUDFLARE.COM\nName Server: SERENA.NS.CLOUDFLARE.COM\nDNSSEC: unsigned\nURL of the ICANN WHOIS Data Problem Reporting System: http://wdprs.internic.net/\n>>> Last update of WHOIS database: 2016-06-22T22:00:00Z <<<\n\nFor more information on Whois status codes, please visit https://www.icann.org/resources/pages/epp-status-codes-2014-06-16-en\n\nThe data contained in GoDaddy.com, LLC's WhoIs database,\nwhile believed by the company to be reliable, is provided \"as is\"\nwith no guarantee or warranties regarding its accuracy.  This\ninformation is provided for the sole purpose of assisting you\nin obtaining information about domain name registration records.\nAny use of this data for any other purpose is expressly forbidden without the prior written\npermission of GoDaddy.com, LLC.  By submitting an inquiry,\nyou agree to these terms of usage and limitations of warranty.  In particular,\nyou agree not to use this data to allow, enable, or otherwise make possible,\ndissemination or collection of this data, in part or in its entirety, for any\npurpose, such as the transmission of unsolicited advertising and\nand solicitations of any kind, including spam.  You further agree\nnot to use this data to enable high volume, automated or robotic electronic\nprocesses designed to collect or compile this data for any purpose,\nincluding mining this data for your own personal or commercial purposes. \n\nPlease note: the registrant of the domain name is specified\nin the \"registrant\" section.  In most cases, GoDaddy.com, LLC \nis not the registrant of domain names listed in this database.",
      "lastLoadedAt": "2016-06-25T08:22:11.800-0700"
    }
  ]
}
*/
