#include "Config.h"

#include <QFile>
#include <QDebug>
#include <QGuiApplication>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#define STRING_PAIR QPair<QString,QString>

Config gConfig;

void Config::fromConfigFile(){
    // get settings from settings file
    QFile file(QGuiApplication::applicationDirPath()+"/AIGSuite.json");
    if(!file.open(QIODevice::ReadOnly)){
        qWarning() << "Couldn't open settings file: " << file.fileName();
        return;
    }

    QJsonObject settings = QJsonDocument::fromJson(file.readAll()).object();
    file.close();

    // getting recents
    QJsonObject recentObj = settings["Recents"].toObject();
    foreach(const QString &key, recentObj.keys())
        recents.insert(key, recentObj[key].toString());

    // getting General settings
    QJsonObject generalObj = settings["General"].toObject();
    general.build_date = generalObj["build_date"].toString();
    general.theme = generalObj["theme"].toString();
    general.s3s = generalObj["s3s"].toString();
    general.isFirstRun = generalObj["isFirstRun"].toBool();
    general.font = generalObj["font"].toInt();

    // getting wordlists
    QJsonObject wordlistObj = settings["Wordlist"].toObject();

    QJsonObject subdomainObj = wordlistObj["subdomain"].toObject();
    foreach(const QString &name, subdomainObj.keys())
        wordlist.subdomain.append({name, subdomainObj[name].toString()});

    QJsonObject tldObj = wordlistObj["tld"].toObject();
    foreach(const QString &name, tldObj.keys())
        wordlist.subdomain.append({name, tldObj[name].toString()});

    // getting about info
    QJsonObject aboutObj = settings["About"].toObject();

    QJsonObject authorsObj = aboutObj["authors"].toObject();
    foreach(const QString &name, authorsObj.keys())
        about.authors.insert(name, authorsObj[name].toString());

    QJsonObject donorsObj = aboutObj["donors"].toObject();
    foreach(const QString &name, donorsObj.keys())
        about.donors.insert(name, donorsObj[name].toString());

    QJsonObject modulesObj = aboutObj["modules"].toObject();
    foreach(const QString &name, modulesObj.keys())
        about.modules.insert(name, modulesObj[name].toString());

    // getting Enums settings
    QJsonObject enumsObj = settings["Enums"].toObject();
    enums.timeout = enumsObj["timeout"].toInt();
    enums.useTimeout = enumsObj["useTimeout"].toBool();
    enums.noDuplicates = enumsObj["noDuplicates"].toBool();
    enums.autoSaveResults = enumsObj["autoSaveResults"].toBool();

    // getting Tools settings
    QJsonObject toolsObj = settings["Tools"].toObject();

    // osint
    QJsonObject osintObj = toolsObj["osint"].toObject();
    tools.osint.timeout = osintObj["timeout"].toInt();
    tools.osint.maxPages = osintObj["maxPages"].toInt();
    tools.osint.useTimeout = osintObj["useTimeout"].toBool();
    tools.osint.noDuplicates = osintObj["noDuplicates"].toBool();
    tools.osint.autoSaveResults = osintObj["autoSaveResults"].toBool();

    // raw
    QJsonObject rawObj = toolsObj["raw"].toObject();
    tools.raw.timeout = rawObj["timeout"].toInt();
    tools.raw.maxPages = rawObj["maxPages"].toInt();
    tools.raw.useTimeout = rawObj["useTimeout"].toBool();
    tools.raw.noDuplicates = rawObj["noDuplicates"].toBool();
    tools.raw.autoSaveResults = rawObj["autoSaveResults"].toBool();

    // brute
    QJsonObject bruteObj = toolsObj["brute"].toObject();
    tools.brute.record = static_cast<Config::DNS>(bruteObj["record"].toInt());
    tools.brute.threads = bruteObj["threads"].toInt();
    tools.brute.timeout = bruteObj["timeout"].toInt();
    tools.brute.wildcard = bruteObj["wildcard"].toBool();
    tools.brute.useTimeout = bruteObj["useTimeout"].toBool();
    tools.brute.noDuplicates = bruteObj["noDuplicates"].toBool();
    tools.brute.autoSaveResults = bruteObj["autoSaveResults"].toBool();
    QJsonObject bruteNSObj = bruteObj["nameservers"].toObject();
    foreach(const QString &key, bruteNSObj.keys())
        tools.brute.nameservers.insert(key, bruteNSObj[key].toBool());

    // host
    QJsonObject hostObj = toolsObj["host"].toObject();
    tools.host.record = static_cast<Config::DNS>(hostObj["record"].toInt());
    tools.host.threads = hostObj["threads"].toInt();
    tools.host.timeout = hostObj["timeout"].toInt();
    tools.host.useTimeout = hostObj["useTimeout"].toBool();
    tools.host.noDuplicates = hostObj["noDuplicates"].toBool();
    tools.host.autoSaveResults = hostObj["autoSaveResults"].toBool();

    QJsonObject hostPingObj = hostObj["ping"].toObject();
    tools.host.ping.ttl = hostPingObj["ttl"].toInt();
    tools.host.ping.bytes = hostPingObj["bytes"].toInt();

    QJsonObject hostNSObj = hostObj["nameservers"].toObject();
    foreach(const QString &key, hostNSObj.keys())
        tools.host.nameservers.insert(key, hostNSObj[key].toBool());

    // ip
    QJsonObject ipToolObj = toolsObj["ip"].toObject();
    tools.ip.record = static_cast<Config::DNS>(ipToolObj["record"].toInt());
    tools.ip.threads = ipToolObj["threads"].toInt();
    tools.ip.timeout = ipToolObj["timeout"].toInt();
    tools.ip.useTimeout = ipToolObj["useTimeout"].toBool();
    tools.ip.noDuplicates = ipToolObj["noDuplicates"].toBool();
    tools.ip.autoSaveResults = ipToolObj["autoSaveResults"].toBool();

    QJsonObject ipPingObj = ipToolObj["ping"].toObject();
    tools.ip.ping.ttl = ipPingObj["ttl"].toInt();
    tools.ip.ping.bytes = ipPingObj["bytes"].toInt();

    QJsonObject ipNSObj = ipToolObj["nameservers"].toObject();
    foreach(const QString &key, ipNSObj.keys())
        tools.ip.nameservers.insert(key, ipNSObj[key].toBool());

    // dns
    QJsonObject dnsObj = toolsObj["dns"].toObject();
    tools.dns.threads = dnsObj["threads"].toInt();
    tools.dns.timeout = dnsObj["timeout"].toInt();
    tools.dns.useTimeout = dnsObj["useTimeout"].toBool();
    tools.dns.noDuplicates = dnsObj["noDuplicates"].toBool();
    tools.dns.autoSaveResults = dnsObj["autoSaveResults"].toBool();

    QJsonObject dnsNSObj = dnsObj["nameservers"].toObject();
    foreach(const QString &key, dnsNSObj.keys())
        tools.dns.nameservers.insert(key, dnsNSObj[key].toBool());

    // ssl
    QJsonObject sslToolObj = toolsObj["ssl"].toObject();
    tools.ssl.threads = sslToolObj["threads"].toInt();
    tools.ssl.timeout = sslToolObj["timeout"].toInt();
    tools.ssl.useTimeout = sslToolObj["useTimeout"].toBool();
    tools.ssl.noDuplicates = sslToolObj["noDuplicates"].toBool();
    tools.ssl.autoSaveResults = sslToolObj["autoSaveResults"].toBool();

    // url
    QJsonObject urlObj = toolsObj["url"].toObject();
    tools.url.threads = urlObj["threads"].toInt();
    tools.url.timeout = urlObj["timeout"].toInt();
    tools.url.useTimeout = urlObj["useTimeout"].toBool();
    tools.url.noDuplicates = urlObj["noDuplicates"].toBool();
    tools.url.autoSaveResults = urlObj["autoSaveResults"].toBool();
    tools.url.getTitle = urlObj["getTitle"].toBool();
    tools.url.forceScheme = urlObj["forceScheme"].toBool();
    tools.url.followRedirects = urlObj["followRedirects"].toBool();
    tools.url.scheme = urlObj["scheme"].toString();

    // API Keys
    QJsonObject keysObj = settings["Keys"].toObject();
    keys.C99 = keysObj["C99"].toString();
    keys.CertSpotter = keysObj["CertSpotter"].toString();
    keys.Github = keysObj["Github"].toString();
    keys.Hunter = keysObj["Hunter"].toString();
    keys.IntelX = keysObj["IntelX"].toString();
    keys.ProjectDiscovery = keysObj["ProjectDiscovery"].toString();
    keys.SecurityTrails = keysObj["SecurityTrails"].toString();
    keys.Shodan = keysObj["Shodan"].toString();
    keys.Spyse = keysObj["Spyse"].toString();
    keys.ThreatBook = keysObj["ThreatBook"].toString();
    keys.VirusTotal = keysObj["VirusTotal"].toString();
    keys.BinaryEdge = keysObj["BinaryEdge"].toString();
    keys.IpInfo = keysObj["IpInfo"].toString();
    keys.WhoIsXMLAPI = keysObj["WhoIsXMLAPI"].toString();
    keys.ZoomEye = keysObj["ZoomEye"].toString();
    keys.IpApi = keysObj["IpApi"].toString();
    keys.ViewDns = keysObj["ViewDns"].toString();
    keys.HackerTarget = keysObj["HackerTarget"].toString();
    keys.WebResolver = keysObj["WebResolver"].toString();
    keys.Mnemonic = keysObj["Mnemonic"].toString();
    keys.Robtex = keysObj["Robtex"].toString();
    keys.Otx = keysObj["Otx"].toString();
    keys.IpRegistry = keysObj["IpRegistry"].toString();
    keys.BuiltWith = keysObj["BuiltWith"].toString();
    keys.Detectify = keysObj["Detectify"].toString();
    keys.DNSlytics = keysObj["DNSlytics"].toString();
    keys.Onyphe = keysObj["Onyphe"].toString();
    keys.FullHunt = keysObj["FullHunt"].toString();
    keys.SpamHaus = keysObj["SpamHaus"].toString();
    keys.Debounce = keysObj["Debounce"].toString();
    keys.HaveIBeenPawned = keysObj["HaveIBeenPawned"].toString();
    keys.Host = keysObj["Host"].toString();
    keys.JsonWhois = keysObj["JsonWhois"].toString();
    keys.LeakIX = keysObj["LeakIX"].toString();
    keys.NetworksDB = keysObj["NetworksDB"].toString();
    keys.Seon = keysObj["Seon"].toString();
    keys.SpyOnWeb = keysObj["SpyOnWeb"].toString();
    keys.WhatCMS = keysObj["WhatCMS"].toString();
    keys.Whoxy = keysObj["Whoxy"].toString();
    keys.EmailCrawlr = keysObj["EmailCrawlr"].toString();
    keys.EmailFormat = keysObj["EmailFormat"].toString();
    keys.EmailRep = keysObj["EmailRep"].toString();
    keys.Snov = keysObj["Snov"].toString();
    keys.IpData = keysObj["IpData"].toString();
    keys.IpGeoLocation = keysObj["IpGeoLocation"].toString();
    keys.IpStack = keysObj["IpStack"].toString();
    keys.AbuseIPDB = keysObj["AbuseIPDB"].toString();
    keys.HybridAnalysis = keysObj["HybridAnalysis"].toString();
    keys.IpQualityScore = keysObj["IpQualityScore"].toString();
    keys.LeakLookup = keysObj["LeakLookup"].toString();
    keys.Wappalyzer = keysObj["Wappalyzer"].toString();
    keys.ZETAlytics = keysObj["ZETAlytics"].toString();
    keys.Ipfy = keysObj["Ipfy"].toString();

    QJsonObject fraudguardObj = keysObj["fraudguard"].toObject();
    keys.fraudguard.user = fraudguardObj["user"].toString();
    keys.fraudguard.pass = fraudguardObj["pass"].toString();

    QJsonObject neutrinoapiObj = keysObj["neutrinoapi"].toObject();
    keys.neutrinoapi.uid = neutrinoapiObj["uid"].toString();
    keys.neutrinoapi.key = neutrinoapiObj["key"].toString();

    QJsonObject DomainToolsObj = keysObj["DomainTools"].toObject();
    keys.DomainTools.username = DomainToolsObj["username"].toString();
    keys.DomainTools.key = DomainToolsObj["key"].toString();

    QJsonObject CirclObj = keysObj["Circl"].toObject();
    keys.Circl.user = CirclObj["user"].toString();
    keys.Circl.pwd = CirclObj["pwd"].toString();

    QJsonObject RiskIQObj = keysObj["RiskIQ"].toObject();
    keys.RiskIQ.key = RiskIQObj["key"].toString();
    keys.RiskIQ.secret = RiskIQObj["secret"].toString();

    QJsonObject CensysObj = keysObj["Censys"].toObject();
    keys.Censys.id = CensysObj["id"].toString();
    keys.Censys.secret = CensysObj["secret"].toString();

    QJsonObject PassiveTotalObj = keysObj["PassiveTotal"].toObject();
    keys.PassiveTotal.key = PassiveTotalObj["key"].toString();
    keys.PassiveTotal.username = PassiveTotalObj["username"].toString();
}

void Config::toConfigFile(){
    // getting recents
    QJsonObject recentObj;
    foreach(const QString &key, recents.keys())
        recentObj.insert(key, recents[key]);

    // general settings
    QJsonObject generalObj;
    generalObj.insert("build_date", general.build_date);
    generalObj.insert("theme", general.theme);
    generalObj.insert("s3s", general.s3s);
    generalObj.insert("isFirstRun", general.isFirstRun);
    generalObj.insert("font", general.font);

    // getting wordlists
    QJsonObject subdomainObj;
    foreach(const STRING_PAIR &name, wordlist.subdomain)
        subdomainObj.insert(name.first, name.second);

    QJsonObject tldObj;
    foreach(const STRING_PAIR &name, wordlist.tld)
        tldObj.insert(name.first, name.second);

    QJsonObject wordlistObj;
    wordlistObj.insert("subdomain", subdomainObj);
    wordlistObj.insert("tld", tldObj);

    // about info
    QJsonObject authorsObj;
    foreach(const QString &name, about.authors.keys())
        authorsObj.insert(name, about.authors[name]);

    QJsonObject donorsObj;
    foreach(const QString &name, about.donors.keys())
        donorsObj.insert(name, about.donors[name]);

    QJsonObject modulesObj;
    foreach(const QString &name, about.modules.keys())
        modulesObj.insert(name, about.modules[name]);

    QJsonObject aboutObj;
    aboutObj.insert("authors", authorsObj);
    aboutObj.insert("donors", donorsObj);
    aboutObj.insert("modules", modulesObj);

    // enums settings
    QJsonObject enumsObj;
    enumsObj.insert("timeout", enums.timeout);
    enumsObj.insert("useTimeout", enums.useTimeout);
    enumsObj.insert("noDuplicates", enums.noDuplicates);
    enumsObj.insert("autoSaveResults", enums.autoSaveResults);

    // set Tools settings
    QJsonObject osintObj;
    osintObj.insert("timeout", tools.osint.timeout);
    osintObj.insert("maxPages", tools.osint.maxPages);
    osintObj.insert("useTimeout", tools.osint.useTimeout);
    osintObj.insert("noDuplicates", tools.osint.noDuplicates);
    osintObj.insert("autoSaveResults", tools.osint.autoSaveResults);

    QJsonObject rawObj;
    rawObj.insert("timeout", tools.raw.timeout);
    rawObj.insert("maxPages", tools.raw.maxPages);
    rawObj.insert("useTimeout", tools.raw.useTimeout);
    rawObj.insert("noDuplicates", tools.raw.noDuplicates);
    rawObj.insert("autoSaveResults", tools.raw.autoSaveResults);

    QJsonObject bruteObj;
    bruteObj.insert("record", tools.brute.record);
    bruteObj.insert("threads", tools.brute.threads);
    bruteObj.insert("timeout", tools.brute.timeout);
    bruteObj.insert("wildcard", tools.brute.wildcard);
    bruteObj.insert("useTimeout", tools.brute.useTimeout);
    bruteObj.insert("noDuplicates", tools.brute.noDuplicates);
    bruteObj.insert("autoSaveResults", tools.brute.autoSaveResults);
    QJsonObject bruteNSObj;
    foreach(const QString &key, tools.brute.nameservers.keys())
        bruteNSObj.insert(key, tools.brute.nameservers[key]);
    bruteObj.insert("nameservers", bruteNSObj);

    QJsonObject ipToolObj;
    ipToolObj.insert("record", tools.ip.record);
    ipToolObj.insert("threads", tools.ip.threads);
    ipToolObj.insert("timeout", tools.ip.timeout);
    ipToolObj.insert("useTimeout", tools.ip.useTimeout);
    ipToolObj.insert("noDuplicates", tools.ip.noDuplicates);
    ipToolObj.insert("autoSaveResults", tools.ip.autoSaveResults);

    QJsonObject ipPingObj;
    ipPingObj.insert("ttl", tools.ip.ping.ttl);
    ipPingObj.insert("bytes", tools.ip.ping.bytes);

    QJsonObject ipNSObj;
    foreach(const QString &key, tools.ip.nameservers.keys())
        ipNSObj.insert(key, tools.ip.nameservers[key]);

    ipToolObj.insert("nameservers", ipNSObj);
    ipToolObj.insert("ping", ipPingObj);

    QJsonObject hostObj;
    hostObj.insert("record", tools.host.record);
    hostObj.insert("threads", tools.host.threads);
    hostObj.insert("timeout", tools.host.timeout);
    hostObj.insert("useTimeout", tools.host.useTimeout);
    hostObj.insert("noDuplicates", tools.host.noDuplicates);
    hostObj.insert("autoSaveResults", tools.host.autoSaveResults);

    QJsonObject hostPingObj;
    hostPingObj.insert("ttl", tools.host.ping.ttl);
    hostPingObj.insert("bytes", tools.host.ping.bytes);

    QJsonObject hostNSObj;
    foreach(const QString &key, tools.host.nameservers.keys())
        hostNSObj.insert(key, tools.host.nameservers[key]);

    hostObj.insert("nameservers", hostNSObj);
    hostObj.insert("ping", hostPingObj);

    QJsonObject dnsObj;
    dnsObj.insert("threads", tools.dns.threads);
    dnsObj.insert("timeout", tools.dns.timeout);
    dnsObj.insert("useTimeout", tools.dns.useTimeout);
    dnsObj.insert("noDuplicates", tools.dns.noDuplicates);
    dnsObj.insert("autoSaveResults", tools.dns.autoSaveResults);

    QJsonObject dnsNSObj;
    foreach(const QString &key, tools.dns.nameservers.keys())
        dnsNSObj.insert(key, tools.dns.nameservers[key]);
    dnsObj.insert("nameservers", dnsNSObj);

    QJsonObject sslToolObj;
    sslToolObj.insert("threads", tools.ssl.threads);
    sslToolObj.insert("timeout", tools.ssl.timeout);
    sslToolObj.insert("useTimeout", tools.ssl.useTimeout);
    sslToolObj.insert("noDuplicates", tools.ssl.noDuplicates);
    sslToolObj.insert("autoSaveResults", tools.ssl.autoSaveResults);

    QJsonObject urlObj;
    urlObj.insert("threads", tools.url.threads);
    urlObj.insert("timeout", tools.url.timeout);
    urlObj.insert("useTimeout", tools.url.useTimeout);
    urlObj.insert("noDuplicates", tools.url.noDuplicates);
    urlObj.insert("autoSaveResults", tools.url.autoSaveResults);
    urlObj.insert("getTitle", tools.url.getTitle);
    urlObj.insert("forceScheme", tools.url.forceScheme);
    urlObj.insert("followRedirects", tools.url.followRedirects);
    urlObj.insert("scheme", tools.url.scheme);

    QJsonObject toolsObj;
    toolsObj.insert("osint", osintObj);
    toolsObj.insert("raw", rawObj);
    toolsObj.insert("brute", bruteObj);
    toolsObj.insert("ip", ipToolObj);
    toolsObj.insert("host", hostObj);
    toolsObj.insert("dns", dnsObj);
    toolsObj.insert("ssl", sslToolObj);
    toolsObj.insert("url", urlObj);

    // set api keys
    QJsonObject keysObj;
    keysObj.insert("C99", keys.C99);
    keysObj.insert("CertSpotter", keys.CertSpotter);
    keysObj.insert("Github", keys.Github);
    keysObj.insert("Hunter", keys.Hunter);
    keysObj.insert("IntelX", keys.IntelX);
    keysObj.insert("ProjectDiscovery", keys.ProjectDiscovery);
    keysObj.insert("SecurityTrails", keys.SecurityTrails);
    keysObj.insert("Shodan", keys.Shodan);
    keysObj.insert("Spyse", keys.Spyse);
    keysObj.insert("ThreatBook", keys.ThreatBook);
    keysObj.insert("VirusTotal", keys.VirusTotal);
    keysObj.insert("BinaryEdge", keys.BinaryEdge);
    keysObj.insert("IpInfo", keys.IpInfo);
    keysObj.insert("WhoIsXMLAPI", keys.WhoIsXMLAPI);
    keysObj.insert("ZoomEye", keys.ZoomEye);
    keysObj.insert("IpApi", keys.IpApi);
    keysObj.insert("ViewDns", keys.ViewDns);
    keysObj.insert("HackerTarget", keys.HackerTarget);
    keysObj.insert("WebResolver", keys.WebResolver);
    keysObj.insert("Mnemonic", keys.Mnemonic);
    keysObj.insert("Robtex", keys.Robtex);
    keysObj.insert("Otx", keys.Otx);
    keysObj.insert("IpRegistry", keys.IpRegistry);
    keysObj.insert("BuiltWith", keys.BuiltWith);
    keysObj.insert("Detectify", keys.Detectify);
    keysObj.insert("DNSlytics", keys.DNSlytics);
    keysObj.insert("Onyphe", keys.Onyphe);
    keysObj.insert("FullHunt", keys.FullHunt);
    keysObj.insert("SpamHaus", keys.SpamHaus);
    keysObj.insert("Debounce", keys.Debounce);
    keysObj.insert("HaveIBeenPawned", keys.HaveIBeenPawned);
    keysObj.insert("Host", keys.Host);
    keysObj.insert("JsonWhois", keys.JsonWhois);
    keysObj.insert("LeakIX", keys.LeakIX);
    keysObj.insert("NetworksDB", keys.NetworksDB);
    keysObj.insert("Seon", keys.Seon);
    keysObj.insert("SpyOnWeb", keys.SpyOnWeb);
    keysObj.insert("WhatCMS", keys.WhatCMS);
    keysObj.insert("Whoxy", keys.Whoxy);
    keysObj.insert("EmailCrawlr", keys.EmailCrawlr);
    keysObj.insert("EmailFormat", keys.EmailFormat);
    keysObj.insert("EmailRep", keys.EmailRep);
    keysObj.insert("Snov", keys.Snov);
    keysObj.insert("IpData", keys.IpData);
    keysObj.insert("IpGeoLocation", keys.IpGeoLocation);
    keysObj.insert("IpStack", keys.IpStack);
    keysObj.insert("AbuseIPDB", keys.AbuseIPDB);
    keysObj.insert("HybridAnalysis", keys.HybridAnalysis);
    keysObj.insert("IpQualityScore", keys.IpQualityScore);
    keysObj.insert("LeakLookup", keys.LeakLookup);
    keysObj.insert("Wappalyzer", keys.Wappalyzer);
    keysObj.insert("ZETAlytics", keys.ZETAlytics);
    keysObj.insert("Ipfy", keys.Ipfy);

    QJsonObject fraudguardObj;
    fraudguardObj.insert("user", keys.fraudguard.user);
    fraudguardObj.insert("pass", keys.fraudguard.pass);
    keysObj.insert("fraudguard", fraudguardObj);

    QJsonObject neutrinoapiObj;
    neutrinoapiObj.insert("uid", keys.neutrinoapi.uid);
    neutrinoapiObj.insert("key", keys.neutrinoapi.key);
    keysObj.insert("neutrinoapi", neutrinoapiObj);

    QJsonObject DomainToolsObj;
    DomainToolsObj.insert("username", keys.DomainTools.username);
    DomainToolsObj.insert("key", keys.DomainTools.key);
    keysObj.insert("DomainTools", DomainToolsObj);

    QJsonObject CirclObj;
    CirclObj.insert("user", keys.Circl.user);
    CirclObj.insert("pwd", keys.Circl.pwd);
    keysObj.insert("Circl", CirclObj);

    QJsonObject RiskIQObj;
    RiskIQObj.insert("key", keys.RiskIQ.key);
    RiskIQObj.insert("secret", keys.RiskIQ.secret);
    keysObj.insert("RiskIQ", RiskIQObj);

    QJsonObject CensysObj;
    CensysObj.insert("id", keys.Censys.id);
    CensysObj.insert("secret", keys.Censys.secret);
    keysObj.insert("Censys", CensysObj);

    QJsonObject PassiveTotalObj;
    PassiveTotalObj.insert("key", keys.PassiveTotal.key);
    PassiveTotalObj.insert("username", keys.PassiveTotal.username);
    keysObj.insert("PassiveTotal", PassiveTotalObj);

    // set settings
    QJsonObject settings;
    settings.insert("Recents", recentObj);
    settings.insert("General", generalObj);
    settings.insert("Wordlist", wordlistObj);
    settings.insert("Enums", enumsObj);
    settings.insert("Tools", toolsObj);
    settings.insert("Keys", keysObj);
    settings.insert("About", aboutObj);

    // save settings
    QFile file(QGuiApplication::applicationDirPath()+"/AIGSuite.json");
    if(!file.open(QIODevice::WriteOnly|QIODevice::Truncate)){
        qWarning() << "Couldn't open settings file: " << file.fileName();
        return;
    }
    QJsonDocument doc;
    doc.setObject(settings);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();
}
