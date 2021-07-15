#ifndef LIBLEVEL_H
#define LIBLEVEL_H

/************************************************
                   includes
************************************************/
#include "src/core.h"
#include "src/dialogs/ConfigDialog.h"
#include "src/dialogs/WordlistDialog.h"

/************************************************
                  structures
************************************************/
struct ScanArguments_level{
    QListWidget* wordlist;
    QListWidget* targetList;
    //...
    int progress;
    int currentTargetToEnumerate;
    int currentWordlistToEnumerate;
    //...
    int currentLevel;
    int maxLevel;
};
typedef ScanArguments_level ScanArguments_level;


#endif // LIBLEVEL_H
