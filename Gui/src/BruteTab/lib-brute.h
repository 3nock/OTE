#ifndef LIBBRUTE_H
#define LIBBRUTE_H

#include "src/core.h"
#include <QProgressBar>

//strcutures...
struct ScanArguments_Brute{
    QStringList target;
    QListWidget *wordlist = nullptr;
    QListWidget *targetList = nullptr;
    int progress = 0;
    //...
    bool tldBrute = false;
    bool subBrute = false;
    //...
    int currentWordlistToEnumerate = 0;
    int currentTargetToEnumerate = 0;
    //...
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool checkWildcardSubdomains = false;
    bool useCustomNameServers = false;
    int maxThreads = 50;
    //...
    bool usesWildcards = false;
    QString foundWildcardIp = nullptr;
};
typedef ScanArguments_Brute ScanArguments_Brute;

//...
#endif // LIBBRUTE_H
