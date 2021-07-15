#ifndef LIBBRUTE_H
#define LIBBRUTE_H

/***************************************
               Includes
****************************************/
#include "src/core.h"
#include "src/dialogs/ConfigDialog.h"
#include "src/dialogs/WordlistDialog.h"
#include "src/dialogs/WordlistGeneratorDialog.h"

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
