#ifndef BRUTE_H
#define BRUTE_H

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/scan/BruteScanner.h"
#include "src/utils/NotesSyntaxHighlighter.h"


namespace Ui {
    class Brute;
}

class Brute : public AbstractEngine{
    Q_OBJECT

    public:
        Brute(QWidget *parent = nullptr,
              ResultsModel *resultsModel = nullptr,
              ProjectDataModel *project = nullptr,
              Status *status = nullptr);
        ~Brute();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void onScanResult(QString subdomain, QString ipAddress, QString target);
        /* ... */
        void onChoosenWordlist(QString);
        /* receiving targets from other engines */
        void onReceiveTargets(ENGINE, RESULT_TYPE, RESULT_MODEL_TYPE);
        void onReceiveTargets(QItemSelectionModel*, RESULT_TYPE, RESULT_MODEL_TYPE);

    private slots:
        void onOpenInBrowser(QItemSelectionModel*);
        void onClearResults();
        void onRemoveResults(QItemSelectionModel*);
        void onSaveResults(RESULT_TYPE, RESULT_MODEL_TYPE);
        void onSaveResults(QItemSelectionModel*);
        void onCopyResults(RESULT_TYPE, RESULT_MODEL_TYPE);
        void onCopyResults(QItemSelectionModel*);
        /* ... */
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonAction_clicked();
        void on_buttonConfig_clicked();
        void on_buttonWordlist_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxMultipleTargets_clicked(bool checked);

    private:
        Ui::Brute *ui;
        QSet<QString> m_subdomainsSet;
        brute::ScanConfig *m_scanConfig;
        brute::ScanArgs *m_scanArgs;
        QStringListModel *m_wordlistModel;
        QStringListModel *m_targetListModel;
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        /* ... */
        void m_stopScan();
        void m_startScan();
        void m_pauseScan();
        void m_resumeScan();
        /* ... */
        void m_initActions();

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};

        /* action button */
        QAction a_SendIpToIpFromSubdomain{"Send Addresses To Ip"};
        QAction a_SendIpToIpFromTld{"Send Addresses To Ip"};
        QAction a_SendIpToOsintFromSubdomain{"Send Addresses To Osint"};
        QAction a_SendIpToOsintFromTld{"Send Addresses To Osint"};
        QAction a_SendIpToRawFromSubdomain{"Send Address To Raw"};
        QAction a_SendIpToRawFromTld{"Send Address To Raw"};
        QAction a_SendHostToOsintFromSubdomain{"Send Hostnames To Osint"};
        QAction a_SendHostToOsintFromTld{"Send Hostnames To Osint"};
        QAction a_SendHostToRawFromSubdomain{"Send Hostnames To Raw"};
        QAction a_SendHostToRawFromTld{"Send Hostnames To Raw"};
        QAction a_SendHostToBruteFromSubdomain{"Send Hostnames To Brute"};
        QAction a_SendHostToBruteFromTld{"Send Hostnames To Brute"};
        QAction a_SendHostToActiveFromSubdomain{"Send Hostnames To Active"};
        QAction a_SendHostToActiveFromTld{"Send Hostnames To Active"};
        QAction a_SendHostToDnsFromSubdomain{"Send Hostnames To Records"};
        QAction a_SendHostToDnsFromTld{"Send Hostnames To Records"};
        QAction a_SendHostToCertFromSubdomain{"Send Hostnames To Cert"};
        QAction a_SendHostToCertFromTld{"Send Hostnames To Cert"};
        QAction a_SendIpToIpToolFromSubdomain{"Send Addresses To IpTool"};
        QAction a_SendIpToIpToolFromTld{"Send Addresses To IpTool"};
        QAction a_SendHostToCertToolFromSubdomain{"Send Hostnames To CertTool"};
        QAction a_SendHostToCertToolFromTld{"Send Hostnames To CertTool"};
        QAction a_SendHostToDomainToolFromSubdomain{"Send Hostnames To DomainTool"};
        QAction a_SendHostToDomainToolFromTld{"Send Hostnames To DomainTool"};
        /* right-click */
        QAction a_SendSelectedIpToIpFromSubdomain{"Send Addresses To Ip"};
        QAction a_SendSelectedIpToIpFromTld{"Send Addresses To Ip"};
        QAction a_SendSelectedIpToOsintFromSubdomain{"Send Addresses To Osint"};
        QAction a_SendSelectedIpToOsintFromTld{"Send Addresses To Osint"};
        QAction a_SendSelectedIpToRawFromSubdomain{"Send Address To Raw"};
        QAction a_SendSelectedIpToRawFromTld{"Send Address To Raw"};
        QAction a_SendSelectedHostToOsintFromSubdomain{"Send Hostnames To Osint"};
        QAction a_SendSelectedHostToOsintFromTld{"Send Hostnames To Osint"};
        QAction a_SendSelectedHostToRawFromSubdomain{"Send Hostnames To Raw"};
        QAction a_SendSelectedHostToRawFromTld{"Send Hostnames To Raw"};
        QAction a_SendSelectedHostToBruteFromSubdomain{"Send Hostnames To Brute"};
        QAction a_SendSelectedHostToBruteFromTld{"Send Hostnames To Brute"};
        QAction a_SendSelectedHostToActiveFromSubdomain{"Send Hostnames To Active"};
        QAction a_SendSelectedHostToActiveFromTld{"Send Hostnames To Active"};
        QAction a_SendSelectedHostToDnsFromSubdomain{"Send Hostnames To Records"};
        QAction a_SendSelectedHostToDnsFromTld{"Send Hostnames To Records"};
        QAction a_SendSelectedHostToCertFromSubdomain{"Send Hostnames To Cert"};
        QAction a_SendSelectedHostToCertFromTld{"Send Hostnames To Cert"};
        QAction a_SendSelectedIpToIpToolFromSubdomain{"Send Addresses To IpTool"};
        QAction a_SendSelectedIpToIpToolFromTld{"Send Addresses To IpTool"};
        QAction a_SendSelectedHostToCertToolFromSubdomain{"Send Hostnames To CertTool"};
        QAction a_SendSelectedHostToCertToolFromTld{"Send Hostnames To CertTool"};
        QAction a_SendSelectedHostToDomainToolFromSubdomain{"Send Hostnames To DomainTool"};
        QAction a_SendSelectedHostToDomainToolFromTld{"Send Hostnames To DomainTool"};

        /* save */
        QAction a_SaveFromSubdomain{"Save"};
        QAction a_SaveFromTld{"Save"};
        QAction a_SaveSubdomainIpFromSubdomain{"subdomain | ip"};
        QAction a_SaveSubdomainIpFromTld{"subdomain | ip"};
        QAction a_SaveSubdomainFromSubdomain{"subdomains"};
        QAction a_SaveSubdomainFromTld{"subdomains"};
        QAction a_SaveIpFromSubdomain{"ip-addresses"};
        QAction a_SaveIpFromTld{"ip-addresses"};
        /* copy */
        QAction a_CopyFromSubdomain{"Copy"};
        QAction a_CopyFromTld{"Copy"};
        QAction a_CopySubdomainIpFromSubdomain{"subdomain | ip"};
        QAction a_CopySubdomainIpFromTld{"subdomain | ip"};
        QAction a_CopySubdomainFromSubdomain{"subdomains"};
        QAction a_CopySubdomainFromTld{"subdomains"};
        QAction a_CopyIpFromSubdomain{"ip-addresses"};
        QAction a_CopyIpFromTld{"ip-addresses"};
};

#endif // BRUTE_H
