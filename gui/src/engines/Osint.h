#ifndef OSINT_H
#define OSINT_H

#include "AbstractEngine.h"
#include "src/utils/utils.h"
#include "src/modules/scan/OsintScanner.h"
#include "src/dialogs/ApiKeysDialog.h"
#include "src/dialogs/ConfigDialog.h"

#define SUBDOMAINIP 0
#define SUBDOMAIN 1
#define IP 2
#define EMAIL 3
#define URL 4
#define ASN 5
#define SSLCERT 6

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
        void onResultIp(QString ip);
        void onResultEmail(QString email);
        void onResultUrl(QString url);
        void onResultAsn(QString asnValue, QString asnName);
        /* dns */
        void onResultA(QString A);
        void onResultAAAA(QString AAAA);
        void onResultCNAME(QString CNAME);
        void onResultMX(QString MX);
        void onResultNS(QString NS);
        void onResultTXT(QString TXT);
        void onResultCertFingerprint(QString certId);

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
        void on_comboBoxOption_currentIndexChanged(int index);
        void on_lineEditFilter_textChanged(const QString &arg1);
        ///
        /// ...
        ///
        void on_useProfiles_clicked(bool checked);
        void on_buttonLoadProfile_clicked();
        void on_buttonCreateProfile_clicked();
        void on_buttonDeleteProfile_clicked();

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
