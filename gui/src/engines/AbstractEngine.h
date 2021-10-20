#ifndef ABSTRACTCLASS_H
#define ABSTRACTCLASS_H
//...
#include <QMenu>
#include <QAction>
#include "src/utils/utils.h"
#include "src/utils/ResultsModels.h"
#include "src/widgets/InputWidget.h"
#include "src/project/ProjectDataModel.h"

enum CHOICE{
    subdomainIp,
    subdomain,
    ip,
    email,
    url,
    asn,
    srv,
    CNAME,
    TXT,
    NS,
    MX,
    A,
    AAAA,
};

struct ScanStatus{
    bool isRunning = false;
    bool isStopped = false;
    bool isPaused = false;
    int activeThreads = 0;
};

class Status{

    public:
        Status()
            : osint(new ScanStatus),
              brute(new ScanStatus),
              active(new ScanStatus),
              ip(new ScanStatus),
              records(new ScanStatus)
        {
        }
        ~Status(){
            delete osint;
            delete brute;
            delete active;
            delete ip;
            delete records;
        }
        ScanStatus *osint;
        ScanStatus *brute;
        ScanStatus *active;
        ScanStatus *ip;
        ScanStatus *records;
        //...
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
        AbstractEngine(QWidget *parent = nullptr,
                       ResultsModel *resultsModel = nullptr,
                       ProjectDataModel *project = nullptr,
                       Status *status = nullptr)
            : QWidget(parent),
              status(status),
              project(project),
              result(resultsModel),
              scanConfig(new ScanConfig)
        {
        }
        ~AbstractEngine()
        {
            delete scanConfig;
        }

    protected:
        Status *status;
        ProjectDataModel *project;
        ResultsModel *result;
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
        void sendIpAddressesToIp(ENGINE, CHOICE, PROXYMODEL_TYPE);
        void sendSubdomainsToBrute(ENGINE, CHOICE, PROXYMODEL_TYPE);
        void sendSubdomainsToOsint(ENGINE, CHOICE, PROXYMODEL_TYPE);
        void sendSubdomainsToActive(ENGINE, CHOICE, PROXYMODEL_TYPE);
        void sendSubdomainsToRecord(ENGINE, CHOICE, PROXYMODEL_TYPE);
        void sendSubdomainsToProject(ENGINE, CHOICE, PROXYMODEL_TYPE);
        //...
        void sendIpAddressesToIp(QItemSelectionModel*);
        void sendSubdomainsToBrute(QItemSelectionModel*);
        void sendSubdomainsToOsint(QItemSelectionModel*);
        void sendSubdomainsToActive(QItemSelectionModel*);
        void sendSubdomainsToRecord(QItemSelectionModel*);
        void sendSubdomainsToProject(QItemSelectionModel*);

    public slots:
        void onReceiveTargets(ENGINE, CHOICE, PROXYMODEL_TYPE);
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
        QAction actionSendToIp_ip{"Send Addresses To Ip"};
        QAction actionSendToOsint_subdomain{"Send Hostnames To Osint"};
        QAction actionSendToActive_subdomain{"Send Hostnames To Active"};
        QAction actionSendToBrute_subdomain{"Send Hostnames To Brute"};
        QAction actionSendToRecords_subdomain{"Send Hostnames To Records"};
        //...
        QAction actionSendToOsint_dns{"Send Hostnames To Osint"};
        QAction actionSendToActive_dns{"Send Hostnames To Active"};
        QAction actionSendToBrute_dns{"Send Hostnames To Brute"};
        QAction actionSendToRecords_dns{"Send Hostnames To Records"};
        //...
        QAction actionSendToOsint_srv{"Send Hostnames To Osint"};
        QAction actionSendToActive_srv{"Send Hostnames To Active"};
        QAction actionSendToBrute_srv{"Send Hostnames To Brute"};
        QAction actionSendToRecords_srv{"Send Hostnames To Records"};
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
        QAction actionSaveEmails{"emails"};
        QAction actionSaveUrls{"urls"};
        QAction actionSaveAsns{"Asns"};
        //...
        QAction actionSaveSubdomains_subdomain{"subdomains"};
        QAction actionSaveIpAddresses_ip{"ip-addresses"};
        // for dns-records...
        QAction actionSaveA{"A Records"};
        QAction actionSaveAAAA{"AAAA Records"};
        QAction actionSaveMX{"MX Records"};
        QAction actionSaveNS{"NS Records"};
        QAction actionSaveCNAME{"CNAME Records"};
        QAction actionSaveTXT{"TXT Records"};
        QAction actionSaveSRV{"SRV Targets"};
        QAction actionSaveSRVName{"SRV Names"};
        ///
        /// copy actions...
        ///
        QAction actionCopy{"Copy"};
        QAction actionCopyAll{"subdomain | ip"};
        QAction actionCopySubdomains{"subdomains"};
        QAction actionCopyIpAddresses{"ip-addresses"};
        QAction actionCopyEmails{"emails"};
        QAction actionCopyUrls{"urls"};
        QAction actionCopyAsns{"Asns"};
        //...
        QAction actionCopySubdomains_subdomain{"subdomains"};
        QAction actionCopyIpAddresses_ip{"ip-addresses"};
        // for dns-records...
        QAction actionCopyA{"A Records"};
        QAction actionCopyAAAA{"AAAA Records"};
        QAction actionCopyMX{"MX Records"};
        QAction actionCopyNS{"NS Records"};
        QAction actionCopyCNAME{"CNAME Records"};
        QAction actionCopyTXT{"TXT Records"};
        QAction actionCopySRV{"SRV Targets"};
        QAction actionCopySRVName{"SRV Names"};
};

#endif // ABSTRACTCLASS_H
