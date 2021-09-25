#include "C99.h"
#include "src/utils/Config.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


C99::C99(QString _target, QObject *parent):
    AbstractOsintModule(parent)
{
    target = _target;
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &C99::replyFinished);
    ///
    /// get api key....
    ///
    Config::generalConfig().beginGroup("api-keys");
    m_key = Config::generalConfig().value("c99").toString();
    Config::generalConfig().endGroup();
}
C99::~C99(){
    delete manager;
}

void C99::start(){
    QNetworkRequest request;
    QUrl url("https://api.c99.nl/subdomainfinder?key="+m_key+"&domain="+target+"&json");
    request.setUrl(url);
    manager->get(request);
}

void C99::replyFinished(QNetworkReply *reply){
    if(reply->error() == QNetworkReply::NoError)
    {
        QJsonDocument jsonReply = QJsonDocument::fromJson(reply->readAll());
        QJsonObject jsonObject = jsonReply.object();
        ///
        /// check if the request was successful...
        ///
        bool success = jsonObject["success"].toBool();
        if(success){
            QJsonArray subdomainList = jsonObject["subdomains"].toArray();
            foreach(const QJsonValue &value, subdomainList)
                emit scanResults(value.toString());
        }
        else{
            QString error = jsonObject["error"].toString();
            emit scanResults(error);
        }
    }
    else{
        emit scanResults(reply->errorString());
    }
    reply->deleteLater();
    emit quitThread();
}

/*      OTHER

Multiple Port Scanner
Scans all ports of given host.
https://api.c99.nl/portscanner?key=<key>&host=1.1.1.1&json

Single Port Scanner
Scans a single port of given host.
https://api.c99.nl/portscanner?key=<key>&host=1.1.1.1&port=80&json

Nmap Scanner
Performs an nmap scan to find open ports.
https://api.c99.nl/nmap?key=<key>&host=1.1.1.1&json

IP to Host
Gets host of given IP address.
https://api.c99.nl/gethostname?key=<key>&host=1.1.1.1&json

IP 2 Domains
Attempts to find websites hosted on the specific IP address.
https://api.c99.nl/ip2domains?key=<key>&ip=1.1.1.1&json

Whois Checker
Whois check on a domain to see who the domain owner is.
https://api.c99.nl/whois?key=<key>&domain=example.com&json

Screenshot Tool
Creates a screenshot of any website/url you want.
https://api.c99.nl/createscreenshot?key=<key>&url=https://example.com/&json

GeoIP
Locates the given host/ip address.
https://api.c99.nl/geoip?key=<key>&host=1.1.1.1&json

Email Validator
Checks if given up e-mail exists.
https://api.c99.nl/emailvalidator?key=<key>&email=example@outlook.com&json

*/
