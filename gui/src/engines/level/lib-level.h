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
namespace level {
    struct ScanArguments{
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
    typedef ScanArguments ScanArguments;
}

#endif // LIBLEVEL_H
