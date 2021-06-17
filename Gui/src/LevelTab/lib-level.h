#ifndef LIBLEVEL_H
#define LIBLEVEL_H

#include "src/core.h"
#include "src/BruteTab/lib-brute.h"

struct ScanArguments_level{
    QStandardItemModel* model_subdomains = nullptr;
    QListWidget* wordlist;
    //...
    int currentWordlistToEnumerate = 0;
    int currentSubdomainToEnumerate = 0;
    //...
    bool usesWildcards = false;
    bool checkWildcardSubdomains = false;
    QString foundWildcardIp = nullptr;
};
typedef ScanArguments_level ScanArguments_level;


#endif // LIBLEVEL_H
