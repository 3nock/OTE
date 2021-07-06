#ifndef LIBLEVEL_H
#define LIBLEVEL_H

#include "src/core.h"
#include "src/BruteTab/lib-brute.h"

struct ScanArguments_level{
    QListWidget* wordlist = nullptr;
    QListWidget* targetList = nullptr;
    //...
    int currentWordlistToEnumerate = 0;
    int currentTargetToEnumerate = 0;
    //...
    int currentLevel = 0;
    int maxLevel = 0;
    //...
    int lastCount = 0;
    //...
    QDnsLookup::Type dnsRecordType = QDnsLookup::A;
    bool useCustomNameServers = false;
    int maxThreads = 50;
    //...
    bool checkWildcardSubdomains = false;
    bool usesWildcards = false;
    QString foundWildcardIp = nullptr;
    //...
    int progress = 0;
};
typedef ScanArguments_level ScanArguments_level;


#endif // LIBLEVEL_H
