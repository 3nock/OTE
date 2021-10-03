#ifndef DNSRECORDS_H
#define DNSRECORDS_H

#include "src/utils/utils.h"
#include "src/engines/AbstractEngine.h"
#include "DnsRecordsScanner.h"

namespace Ui {
    class DnsRecords;
}

class DnsRecords : public AbstractEngine{
    Q_OBJECT

    public:
        DnsRecords(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr, Status *status = nullptr);
        ~DnsRecords();

    public slots:
        void onScanThreadEnded();
        void onInfoLog(QString log);
        void onErrorLog(QString log);
        void onScanResult(records::Results);

    private slots:
        void onShowFilter(bool);
        void onExpandResultsDnsRecords();
        void onCollapseResultsDnsRecords();
        void onClearResultsDnsRecords();
        void onClearResultsSrvRecords();
        void onSaveResultsDnsRecords(CHOICE);
        void onSaveResultsDnsRecords(QItemSelectionModel*);
        void onCopyResultsDnsRecords(CHOICE);
        void onCopyResultsDnsRecords(QItemSelectionModel*);
        void onSaveResultsSrvRecords(CHOICE);
        void onSaveResultsSrvRecords(QItemSelectionModel*);
        void onCopyResultsSrvRecords(CHOICE);
        void onCopyResultsSrvRecords(QItemSelectionModel*);
        ///
        /// ....
        ///
        void on_buttonAction_clicked();
        void on_buttonStart_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_comboBoxOption_currentIndexChanged(int index);
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_tableViewSRV_customContextMenuRequested(const QPoint &pos);

        void on_buttonFilter_clicked();

private:
        Ui::DnsRecords *ui;
        records::ScanArguments* m_scanArguments;
        QStandardItemModel *m_modelDnsRecords;
        QStandardItemModel *m_modelSrvRecords;
        QSortFilterProxyModel *m_proxyModelDnsRecords;
        QSortFilterProxyModel *m_proxyModelSrvRecords;
        void stopScan();
        void startScan();
        void pauseScan();
        void ResumeScan();
        void loadSrvWordlist();
        void connectActions();
};

#endif // DNSRECORDS_H
