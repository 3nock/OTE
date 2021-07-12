#ifndef LIBBRUTE_H
#define LIBBRUTE_H

/***************************************
               Includes
****************************************/
#include "src/core.h"
#include "src/Dialogs/ConfigDialog.h"
#include "src/Dialogs/WordlistDialog.h"
#include "src/Dialogs/WordlistGeneratorDialog.h"

/***************************************
              strcutures
****************************************/
namespace brute {
    struct ScanArguments{
        QStringList targetList;
        QListWidget *wordlist;
        //...
        bool tldBrute;
        bool subBrute;
        //...
        int currentWordlistToEnumerate;
        int currentTargetToEnumerate;
        int progress;
    };
    typedef ScanArguments ScanArguments;
}

#endif // LIBBRUTE_H
