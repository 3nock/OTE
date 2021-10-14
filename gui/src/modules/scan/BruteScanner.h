#ifndef BRUTESCANNER_H
#define BRUTESCANNER_H

#include "AbstractScanner.h"
#include "src/dialogs/ConfigDialog.h"
#include "src/dialogs/WordlistDialog.h"


namespace brute {

struct ScanArguments{
    QStringList targetList;
    QStringList wordlist;
    //...
    bool tldBrute;
    bool subBrute;
    //...
    int currentWordlistToEnumerate;
    int currentTargetToEnumerate;
    int progress;
};
typedef ScanArguments ScanArguments;


class Scanner : public AbstractScanner{
    Q_OBJECT

    public:
        Scanner(ScanConfig *scanConfig, brute::ScanArguments *scanArguments);
        ~Scanner() override;

    private slots:
        void lookup() override;
        void lookupFinished();

    signals:
        void anotherLookup();
        void scanResult(QString subdomain, QString ipAddress, QString target);

    private:
        int m_currentWordlistToEnumerate = 0;
        int m_currentTargetToEnumerate = 0;
        //...
        brute::ScanArguments *m_scanArguments;
        ScanConfig *m_scanConfig;
        QDnsLookup *m_dns;
};

}
#endif //BRUTE_H
