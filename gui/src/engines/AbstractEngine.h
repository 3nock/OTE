#ifndef ABSTRACTCLASS_H
#define ABSTRACTCLASS_H

#include <QMenu>
#include <QAction>
#include "src/utils/utils.h"
#include "src/widgets/InputWidget.h"

enum CHOICE{
    susbdomains = 0,
    ipaddress = 1,
    all = 2,
    //...
    srvName = 3,
    srvTarget = 4,
    //...
    CNAME = 5,
    TXT = 6,
    NS = 7,
    MX = 8,
    A = 9,
    AAAA = 10
};

struct ScanStatus{
    bool isRunning = false;
    bool isStopped = false;
    bool isPaused = false;
    int activeThreads = 0;
};

struct Status{
    ScanStatus *osint = nullptr;
    ScanStatus *brute = nullptr;
    ScanStatus *active = nullptr;
    ScanStatus *ip = nullptr;
    ScanStatus *records = nullptr;
    int totalThreadsInUse(){
        return osint->activeThreads+
                brute->activeThreads+
                active->activeThreads+
                ip->activeThreads+
                records->activeThreads;
    }
};

class AbstractEngine : public QWidget{
        Q_OBJECT

    public:
        AbstractEngine(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr, Status *status = nullptr)
            : QWidget(parent),
              status(status),
              resultsModel(resultsModel),
              scanConfig(new ScanConfig)
        {
        }
        ~AbstractEngine()
        {
            delete scanConfig;
        }

    protected:
        Status *status;
        ResultsModel *resultsModel;
        ScanConfig *scanConfig;
        InputWidget* targets = nullptr;
        QItemSelectionModel *selectionModel = nullptr;

    signals:
        ///
        /// signals to scanning threads
        ///
        void stopScanThread();
        void pauseScanThread();
        void resumeScanThread();
        ///
        /// signals to the mainwindow...
        ///
        void sendNotes(QString notes);
        void sendStatus(QString status);
        ///
        /// tab change...
        ///
        void changeTabToIp();
        void changeTabToOsint();
        void changeTabToBrute();
        void changeTabToActive();
        void changeTabToRecords();
        void changeTabToProject();

/*
 *
 * For Engine's Context Menu...
 *
 */
    signals:
        void sendIpAddressesToIp(ENGINE, CHOICE);
        void sendSubdomainsToBrute(ENGINE, CHOICE);
        void sendSubdomainsToOsint(ENGINE, CHOICE);
        void sendSubdomainsToActive(ENGINE, CHOICE);
        void sendSubdomainsToRecord(ENGINE, CHOICE);
        void sendSubdomainsToProject(ENGINE, CHOICE);
        //...
        void sendIpAddressesToIp(QItemSelectionModel*);
        void sendSubdomainsToBrute(QItemSelectionModel*);
        void sendSubdomainsToOsint(QItemSelectionModel*);
        void sendSubdomainsToActive(QItemSelectionModel*);
        void sendSubdomainsToRecord(QItemSelectionModel*);
        void sendSubdomainsToProject(QItemSelectionModel*);

    public slots:
        void onReceiveTargets(ENGINE, CHOICE);
        void onReceiveTargets(QItemSelectionModel*);
        void openInBrowser(QItemSelectionModel*);

    protected:
        QAction actionClearResults{"Clear Results"};
        QAction actionExpandResults{"Expand Results"};
        QAction actionCollapseResults{"Collapse Results"};
        QAction actionShowFilter{"Show Filter"};
        QAction actionHideFilter{"Hide Filter"};
        ///
        ///...
        ///
        QAction actionSendToIp{"Send Addresses To Ip"};
        QAction actionSendToOsint{"Send Hostnames To Osint"};
        QAction actionSendToActive{"Send Hostnames To Active"};
        QAction actionSendToBrute{"Send Hostnames To Brute"};
        QAction actionSendToRecords{"Send Hostnames To Records"};
        //...
        QAction actionOpenInBrowser{"Open in Browser"};
        //...
        QAction actionSendToIp_c{"Send Selected To Ip"};
        QAction actionSendToOsint_c{"Send Selected To Osint"};
        QAction actionSendToActive_c{"Send Selected To Active"};
        QAction actionSendToBrute_c{"Send Selected To Brute"};
        QAction actionSendToRecords_c{"Send Selected To Records"};
        QAction actionOpenInBrowser_c{"Open Selected in Browser"};
        ///
        /// save actions...
        ///
        QAction actionSave{"Save"};
        QAction actionSaveAll{"subdomain | ip"};
        QAction actionSaveSubdomains{"subdomains"};
        QAction actionSaveIpAddresses{"ip-addresses"};
        // for dns-records...
        QAction actionSaveA{"A Records"};
        QAction actionSaveAAAA{"AAAA Records"};
        QAction actionSaveMX{"MX Records"};
        QAction actionSaveNS{"NS Records"};
        QAction actionSaveCNAME{"CNAME Records"};
        QAction actionSaveTXT{"TXT Records"};
        QAction actionSaveSRVTarget{"SRV Targets"};
        QAction actionSaveSRVName{"SRV Names"};
        ///
        /// copy actions...
        ///
        QAction actionCopy{"Copy"};
        QAction actionCopyAll{"subdomain | ip"};
        QAction actionCopySubdomains{"subdomains"};
        QAction actionCopyIpAddresses{"ip-addresses"};
        // for dns-records...
        QAction actionCopyA{"A Records"};
        QAction actionCopyAAAA{"AAAA Records"};
        QAction actionCopyMX{"MX Records"};
        QAction actionCopyNS{"NS Records"};
        QAction actionCopyCNAME{"CNAME Records"};
        QAction actionCopyTXT{"TXT Records"};
        QAction actionCopySRVTarget{"SRV Targets"};
        QAction actionCopySRVName{"SRV Names"};
};

#endif // ABSTRACTCLASS_H
