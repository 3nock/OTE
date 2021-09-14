#ifndef DNSRECORDS_H
#define DNSRECORDS_H

#include "src/utils.h"
#include "src/engines/Base.h"
#include "DnsRecordsScanner.h"

namespace Ui {
    class DnsRecords;
}

class DnsRecords : public BaseClass{
    Q_OBJECT

    public:
        DnsRecords(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~DnsRecords();

    public slots:
        void scanThreadEnded();
        void scanResult(records::Results);

    private slots:
        void on_buttonAction_clicked();
        void on_buttonStart_clicked();
        void on_buttonPause_clicked();
        void on_buttonStop_clicked();
        void on_buttonConfig_clicked();
        void on_comboBoxOption_currentIndexChanged(int index);
        //...
        void on_buttonClearResults_clicked();
        void on_treeViewResults_customContextMenuRequested(const QPoint &pos);
        void on_tableViewSRV_customContextMenuRequested(const QPoint &pos);

    private:
        Ui::DnsRecords *ui;
        //...
        void startScan();
        void loadSrvWordlist();
        records::ScanArguments* m_scanArguments;
};

#endif // DNSRECORDS_H
