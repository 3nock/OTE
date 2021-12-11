#ifndef C99_H
#define C99_H

#include "../AbstractOsintModule.h"


namespace ModuleInfo {
struct C99{
    QString name = "C99";
    QString url = "https://api.c99.nl/";
    QString url_apiDoc = "https://api.c99.nl/";
    QString summary = "Quality APIs for Osint";
    QMap <QString, QStringList> flags = {{"Subdomain Finder",
                                          {PLACEHOLDERTEXT_DOMAIN, "Performs an advanced subdomain scan to find most subdomains of the given domain."}},
                                         {"Phone Lookup",
                                          {PLACEHOLDERTEXT_PHONE, "Get more information about any phone number."}},
                                         {"Skype Resolver",
                                          {PLACEHOLDERTEXT_USERNAME, "Attempts to grab an IP address of a Skype account, results are not guaranteed."}},
                                         {"Ip 2 Skype",
                                          {PLACEHOLDERTEXT_IP, "Shows all Skype accounts with that specific IP."}},
                                         {"FirewsubdomainIp Technology Detector",
                                          {PLACEHOLDERTEXT_DOMAIN, "Scans to detect if a website is behind a firewall and determines what firewall it is."}},
                                         {"Multiple port scanner",
                                          {PLACEHOLDERTEXT_IP, "Scans all ports of given host."}},
                                         {"Nmap Scanner",
                                          {PLACEHOLDERTEXT_IP, "Performs an nmap scan to find open ports."}},
                                         {"Ping",
                                          {PLACEHOLDERTEXT_IP, "Pings a host 4 times and shows the result."}},
                                         {"IP to Host",
                                          {PLACEHOLDERTEXT_IP, "Gets host of given IP address."}},
                                         {"Host to IP",
                                          {PLACEHOLDERTEXT_DOMAIN, "Gets server IP of given host."}},
                                         {"Domain Checker",
                                          {PLACEHOLDERTEXT_DOMAIN, "Checks whether a domain is available or not, no matter what extension."}},
                                         {"Domain History Check",
                                          {PLACEHOLDERTEXT_DOMAIN, "Checks available IP history of a domain with dates."}},
                                         {"IP 2 Domains",
                                          {PLACEHOLDERTEXT_IP, "Attempts to find websites hosted on the specific IP address."}},
                                         {"Alexa Rank Checker",
                                          {PLACEHOLDERTEXT_DOMAIN, "Grab the latest Alexa Rank information of any website, including backlinks and top country."}},
                                         {"Whois Checker",
                                          {PLACEHOLDERTEXT_DOMAIN, "Whois check on a domain to see who the domain owner is."}},
                                         {"Screenshot Tool",
                                          {PLACEHOLDERTEXT_DOMAIN, "Creates a screenshot of any website/url you want."}},
                                         {"GeoIP",
                                          {PLACEHOLDERTEXT_IP, "Locates the given host/ip address."}},
                                         {"Website Up or Down Checker",
                                          {PLACEHOLDERTEXT_DOMAIN, "Checks if a host is up or down."}},
                                         {"Site/URL Reputation Checker",
                                          {PLACEHOLDERTEXT_URL, "Checks whether a specific site/url is suspicious and might contain malware, phishing or other potential harmful software."}},
                                         {"Website Headers",
                                          {PLACEHOLDERTEXT_URL, "Gets the headers of a website."}},
                                         {"Link Backup",
                                          {PLACEHOLDERTEXT_URL, "Makes an online backup of a link."}},
                                         {"Email Validator",
                                          {PLACEHOLDERTEXT_EMAIL, "Checks if given up e-mail exists."}},
                                         {"Disposable Mail Check",
                                          {PLACEHOLDERTEXT_EMAIL, "Checks if given up e-mail is disposable/temporarily or not."}},
                                         {"IP Validator",
                                          {PLACEHOLDERTEXT_IP, "Checks if IP address format is valid."}},
                                         {"TOR Checker",
                                          {PLACEHOLDERTEXT_IP, "Checks whether an IP is from a TOR network or not."}},
                                         {"IP Logger",
                                          {PLACEHOLDERTEXT_NONE, "Creates you various links which you can send to anyone to log their IP."}},
                                         {"Proxy Detector",
                                          {PLACEHOLDERTEXT_IP, "Detects whether an IP address is a proxy/vpn or not. High accuracy!"}}};

    QMap<int, QList<int>> input_output = {{IN_DOMAIN,
                                           {OUT_SUBDOMAIN}}}; // others not yet done...
};
}

class C99: public AbstractOsintModule{

    public:
        C99(ScanArgs *args);
        ~C99() override;

    public slots:
        void start() override;
        void replyFinishedSubdomain(QNetworkReply *reply) override;

    private:
        QString m_key = nullptr;
};

#endif // C99_H
