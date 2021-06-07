#ifndef LIBBRUTE_H
#define LIBBRUTE_H

#include "src/core.h"

//macros...
#define WORDLIST_SPECIAL_SUBBRUTE "/wordlists/special_subBrute/names.txt"
#define WORDLIST_SPECIAL_TLDBRUTE "/wordlists/special_tldBrute/names.txt"
#define WORDLIST_NAMESERVERS "/wordlists/nameservers.txt"
#define WORDLIST_CUSTOM_NAMESERVERS "/wordlists/custom_nameservers.txt"
#define WORDLIST_SHORT_SUBBRUTE "/wordlists/subBrute_Short.txt"
#define WORDLIST_SHORT_TLDBRUTE "/wordlists/tldBrute_Short.txt"
#define WORDLIST_MEDIUM_SUBBRUTE "/wordlists/subBrute_Medium.txt"
#define WORDLIST_MEDIUM_TLDBRUTE "/wordlists/tldBrute_Medium.txt"
#define WORDLIST_LONG_SUBBRUTE "/wordlists/subBrute_Long.txt"
#define WORDLIST_LONG_TLDBRUTE "/wordlists/tldBrute_Long.txt"

//strcutures...
struct ScanArguments{
    QString targetDomain = nullptr;
    QListWidget *wordlist = nullptr;
    int enumeratedWordlists = 0;
    //...
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool checkWildcardSubdomains = false;
    bool useCustomNameServers = false;
    int maxThreads = 50;
    //...
    bool usesWildcards = false;
    QString foundWildcardIp = nullptr;
};
typedef ScanArguments ScanArguments;

//functions...
QString nameProcessor_subBrute(QString subdomainName, QString domainName);
QString nameProcessor_tldBrute(QString tldName, QString domainName);
QHostAddress RandomNameserver(bool useCustomNameservers);

#endif // LIBBRUTE_H
