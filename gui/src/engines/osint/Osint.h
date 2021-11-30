#ifndef OSINT_H
#define OSINT_H

#include "../AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/scan/OsintScanner.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/ConfigDialog.h"

/* output option */
#define OUTPUT_SUBDOMAINIP 0
#define OUTPUT_SUBDOMAIN 1
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
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void onResultSubdomainIp(QString subdomain, QString ip);
        void onResultSubdomain(QString subdomain);
        void onResultMightContainWildcards(QString subdomain);
        void onResultIp(QString ip);
        void onResultEmail(QString email);
        void onResultUrl(QString url);
        void onResultAsn(QString asnValue, QString asnName);
        void onResultCidr(QString cidr);
        /* dns */
        void onResultA(QString A);
        void onResultAAAA(QString AAAA);
        void onResultCNAME(QString CNAME);
        void onResultMX(QString MX);
        void onResultNS(QString NS);
        void onResultTXT(QString TXT);
        void onResultSSLCert(QString sha1_or_sha256);

    private slots:
        void onClearResults();
        void onSaveResults(CHOICE, PROXYMODEL_TYPE);
        void onSaveResults(QItemSelectionModel*);
        void onCopyResults(CHOICE, PROXYMODEL_TYPE);
        void onCopyResults(QItemSelectionModel*);
        ///
        /// ....
        ///
        void on_buttonStart_clicked();
        void on_buttonAction_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_buttonKeys_clicked();
        void on_lineEditTarget_returnPressed();
        void on_tableViewResults_customContextMenuRequested(const QPoint &pos);
        void on_checkBoxMultipleTargets_clicked(bool checked);
        void on_comboBoxOutput_currentIndexChanged(int index);
        void on_lineEditFilter_textChanged(const QString &arg1);
        ///
        /// ...
        ///
        void on_useProfiles_clicked(bool checked);
        void on_buttonLoadProfile_clicked();
        void on_buttonCreateProfile_clicked();
        void on_buttonDeleteProfile_clicked();

        void on_comboBoxInput_currentIndexChanged(int index);

    private:
        Ui::Osint *ui;
        //...
        osint::ScanArguments *m_scanArguments;
        osint::ScanResults *m_scanResults;
        //...
        QString m_currentPath;
        QSet<QString> m_subdomainIpSet;
        QSet<QString> m_subdomainSet;
        QSet<QString> m_ipSet;
        QSet<QString> m_emailSet;
        QSet<QString> m_urlSet;
        QSet<QString> m_asnSet;
        QSet<QString> m_sslCertSet;
        QSet<QString> m_cidrSet;
        void stopScan();
        void startScan();
        void pauseScan();
        void ResumeScan();
        void connectActions();
        ///
        ///...
        ///
        void initProfiles();
};

#endif // OSINT_H
