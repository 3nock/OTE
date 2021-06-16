#ifndef LIBLEVEL_H
#define LIBLEVEL_H

#include "src/core.h"

//structures...
struct ScanArguments_level{
    QStandardItemModel* model_subdomains = nullptr;
    QListWidget* wordlist;
    //...
    int subdomainsCount = 0;

}; typedef ScanArguments_level ScanArguments_level;

struct ScanResults_level{
    QStandardItemModel* model_results = nullptr;
    int resultsCount = 0;
}; typedef ScanResults_level ScanResults_level;

#endif // LIBLEVEL_H
