#ifndef OSINT_H
#define OSINT_H

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/ActiveConfigDialog.h"
#include "src/modules/osint/AbstractOsintModule.h"
#include "src/utils/NotesSyntaxHighlighter.h"


/* output option */
#define OUTPUT_SUBDOMAIN 0
#define OUTPUT_SUBDOMAINIP 1
#define OUTPUT_IP 2
#define OUTPUT_EMAIL 3
#define OUTPUT_URL 4
#define OUTPUT_ASN 5
#define OUTPUT_SSLCERT 6
#define OUTPUT_CIDR 7

/* input option */
#define INPUT_DOMAIN 0
#define INPUT_IP 1
#define INPUT_EMAIL 2
#define INPUT_URL 3
#define INPUT_ASN 4
#define INPUT_SSLCERT 5
#define INPUT_CIDR 6


namespace Ui {
    class Osint;
}

class Osint : public AbstractEngine{
    Q_OBJECT

    public:
        Osint(QWidget *parent = nullptr,
              ResultsModel *resultsModel = nullptr,
              ProjectDataModel *project = nullptr,
              Status *status = nullptr);
        ~Osint();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(ScanLog log);
        void onErrorLog(ScanLog log);
        void onRateLimitLog(ScanLog log);
        /* ... */
        void onResultSubdomainIp(QString subdomain, QString ip);
        void onResultSubdomain(QString subdomain);
        void onResultMightContainWildcards(QString subdomain);
        void onResultIp(QString ip);
        void onResultEmail(QString email);
        void onResultUrl(QString url);
        void onResultAsn(QString asnValue, QString asnName);
        void onResultCidr(QString cidr);
        void onResultA(QString A);
        void onResultAAAA(QString AAAA);
        void onResultCNAME(QString CNAME);
        void onResultMX(QString MX);
        void onResultNS(QString NS);
        void onResultTXT(QString TXT);
        void onResultSSLCert(QString sha1_or_sha256);
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
        void on_buttonAction_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_buttonKeys_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxMultipleTargets_clicked(bool checked);
        void on_comboBoxOutput_currentIndexChanged(int index);
        void on_comboBoxInput_currentIndexChanged(int index);
        void on_lineEditFilter_textChanged(const QString &arg1);
        /* ... */
        void on_useProfiles_clicked(bool checked);
        void on_buttonLoadProfile_clicked();
        void on_buttonCreateProfile_clicked();
        void on_buttonDeleteProfile_clicked();

    private:
        Ui::Osint *ui;
        QStringListModel *m_targetListModelHostname;
        QStringListModel *m_targetListModelIp;
        QStringListModel *m_targetListModelAsn;
        QStringListModel *m_targetListModelCidr;
        QStringListModel *m_targetListModelCert;
        QStringListModel *m_targetListModelEmail;
        NotesSyntaxHighlighter *m_notesSyntaxHighlighter;
        /* ... */
        QString m_currentPath;
        QSet<QString> m_subdomainIpSet;
        QSet<QString> m_subdomainSet;
        QSet<QString> m_ipSet;
        QSet<QString> m_emailSet;
        QSet<QString> m_urlSet;
        QSet<QString> m_asnSet;
        QSet<QString> m_sslCertSet;
        QSet<QString> m_cidrSet;
        /* ... */
        void m_stopScan();
        void m_startScan();
        void m_pauseScan();
        void m_resumeScan();
        void m_startScanThread(AbstractOsintModule *);
        /* ... */
        void m_initActions();
        void m_initProfiles();
        void m_initModules();
        /* ... */
        void m_infoLog(QString log);
        void m_errorLog(QString log);

    protected:
        /* general actions */
        QAction a_RemoveResults{"Remove"};
        QAction a_ClearResults{"Clear Results"};
        QAction a_OpenInBrowser{"Open in Browser"};

        /* action button */
        QAction a_SendIpToIpFromIp{"Send Addresses To Ip"};
        QAction a_SendIpToIpFromSubdomainIp{"Send Addresses To Ip"};
        QAction a_SendIpToOsintFromIp{"Send Addresses To Osint"};
        QAction a_SendIpToOsintFromSubdomainIp{"Send Addresses To Osint"};
        QAction a_SendIpToRawFromIp{"Send Address To Raw"};
        QAction a_SendIpToRawFromSubdomainIp{"Send Address To Raw"};
        QAction a_SendHostToOsintFromSubdomain{"Send Hostnames To Osint"};
        QAction a_SendHostToOsintFromSubdomainIp{"Send Hostnames To Osint"};
        QAction a_SendHostToRawFromSubdomain{"Send Hostnames To Raw"};
        QAction a_SendHostToRawFromSubdomainIp{"Send Hostnames To Raw"};
        QAction a_SendHostToBruteFromSubdomain{"Send Hostnames To Brute"};
        QAction a_SendHostToBruteFromSubdomainIp{"Send Hostnames To Brute"};
        QAction a_SendHostToActiveFromSubdomain{"Send Hostnames To Active"};
        QAction a_SendHostToActiveFromSubdomainIp{"Send Hostnames To Active"};
        QAction a_SendHostToDnsFromSubdomain{"Send Hostnames To Records"};
        QAction a_SendHostToDnsFromSubdomainIp{"Send Hostnames To Records"};
        QAction a_SendHostToCertFromSubdomain{"Send Hostnames To Cert"};
        QAction a_SendHostToCertFromSubdomainIp{"Send Hostnames To Cert"};
        QAction a_SendEmailToRaw{"Send Emails To Raw"};
        QAction a_SendUrlToRaw{"Send Urls To Raw"};
        QAction a_SendAsnToRaw{"Send ASNs To Raw"};
        QAction a_SendCertToRaw{"Send Cert To Raw"};
        QAction a_SendCidrToRaw{"Send Cidr To Raw"};
        QAction a_SendIpToIpToolFromIp{"Send Addresses To IpTool"};
        QAction a_SendIpToIpToolFromSubdomainIp{"Send Addresses To IpTool"};
        QAction a_SendHostToCertToolFromSubdomain{"Send Hostnames To CertTool"};
        QAction a_SendHostToCertToolFromSubdomainIp{"Send Hostnames To CertTool"};
        QAction a_SendHostToDomainToolFromSubdomain{"Send Hostnames To DomainTool"};
        QAction a_SendHostToDomainToolFromSubdomainIp{"Send Hostnames To DomainTool"};
        QAction a_SendAsnToAsnTool{"Send ASN to ASNTool"};
        QAction a_SendCertToCertTool{"Send Cert To CertTool"};
        QAction a_SendCidrToCidrTool{"Send Cidr To CidrTool"};
        QAction a_SendEmailToEmailTool{"Send Emails To EmailTool"};
        /* right-click */
        QAction a_SendSelectedIpToIpFromIp{"Send Addresses To Ip"};
        QAction a_SendSelectedIpToIpFromSubdomainIp{"Send Addresses To Ip"};
        QAction a_SendSelectedIpToOsintFromIp{"Send Addresses To Osint"};
        QAction a_SendSelectedIpToOsintFromSubdomainIp{"Send Addresses To Osint"};
        QAction a_SendSelectedIpToRawFromIp{"Send Address To Raw"};
        QAction a_SendSelectedIpToRawFromSubdomainIp{"Send Address To Raw"};
        QAction a_SendSelectedHostToOsintFromSubdomain{"Send Hostnames To Osint"};
        QAction a_SendSelectedHostToOsintFromSubdomainIp{"Send Hostnames To Osint"};
        QAction a_SendSelectedHostToRawFromSubdomain{"Send Hostnames To Raw"};
        QAction a_SendSelectedHostToRawFromSubdomainIp{"Send Hostnames To Raw"};
        QAction a_SendSelectedHostToBruteFromSubdomain{"Send Hostnames To Brute"};
        QAction a_SendSelectedHostToBruteFromSubdomainIp{"Send Hostnames To Brute"};
        QAction a_SendSelectedHostToActiveFromSubdomain{"Send Hostnames To Active"};
        QAction a_SendSelectedHostToActiveFromSubdomainIp{"Send Hostnames To Active"};
        QAction a_SendSelectedHostToDnsFromSubdomain{"Send Hostnames To Records"};
        QAction a_SendSelectedHostToDnsFromSubdomainIp{"Send Hostnames To Records"};
        QAction a_SendSelectedHostToCertFromSubdomain{"Send Hostnames To Cert"};
        QAction a_SendSelectedHostToCertFromSubdomainIp{"Send Hostnames To Cert"};
        QAction a_SendSelectedEmailToRaw{"Send Emails To Raw"};
        QAction a_SendSelectedUrlToRaw{"Send Urls To Raw"};
        QAction a_SendSelectedAsnToRaw{"Send ASNs To Raw"};
        QAction a_SendSelectedCertToRaw{"Send Cert To Raw"};
        QAction a_SendSelectedCidrToRaw{"Send Cidr To Raw"};
        QAction a_SendSelectedIpToIpToolFromIp{"Send Addresses To IpTool"};
        QAction a_SendSelectedIpToIpToolFromSubdomainIp{"Send Addresses To IpTool"};
        QAction a_SendSelectedHostToCertToolFromSubdomain{"Send Hostnames To CertTool"};
        QAction a_SendSelectedHostToCertToolFromSubdomainIp{"Send Hostnames To CertTool"};
        QAction a_SendSelectedHostToDomainToolFromSubdomain{"Send Hostnames To DomainTool"};
        QAction a_SendSelectedHostToDomainToolFromSubdomainIp{"Send Hostnames To DomainTool"};
        QAction a_SendSelectedAsnToAsnTool{"Send ASN to ASNTool"};
        QAction a_SendSelectedCertToCertTool{"Send Cert To CertTool"};
        QAction a_SendSelectedCidrToCidrTool{"Send Cidr To CidrTool"};
        QAction a_SendSelectedEmailToEmailTool{"Send Emails To EmailTool"};

        /* save */
        QAction a_Save{"Save"};
        QAction a_SaveSubdomainIp{"subdomain | ip"};
        QAction a_SaveSubdomainFromSubdomain{"subdomains"};
        QAction a_SaveSubdomainFromSubdomainIp{"subdomains"};
        QAction a_SaveIpFromIp{"ip-addresses"};
        QAction a_SaveIpFromSubdomainIp{"ip-addresses"};
        QAction a_SaveEmail{"Emails"};
        QAction a_SaveUrl{"Urls"};
        QAction a_SaveAsn{"ASNs"};
        QAction a_SaveCert{"Certs"};
        QAction a_SaveCidr{"Cidr"};
        /* copy */
        QAction a_Copy{"Copy"};
        QAction a_CopySubdomainIp{"subdomain | ip"};
        QAction a_CopySubdomainFromSubdomain{"subdomains"};
        QAction a_CopySubdomainFromSubdomainIp{"subdomains"};
        QAction a_CopyIpFromIp{"ip-addresses"};
        QAction a_CopyIpFromSubdomainIp{"ip-addresses"};
        QAction a_CopyEmail{"Emails"};
        QAction a_CopyUrl{"Urls"};
        QAction a_CopyAsn{"ASNs"};
        QAction a_CopyCert{"Certs"};
        QAction a_CopyCidr{"Cidr"};
};

#endif // OSINT_H
