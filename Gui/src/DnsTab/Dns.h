#ifndef DNS_H
#define DNS_H

// headers...
#include "lib-dns.h"
#include "DnsEnumerator.h"

namespace Ui {
    class Dns;
}

class Dns : public QDialog{
    Q_OBJECT

    private:
        Ui::Dns *ui;

    private:
        void startEnumeration();
        //...
        QStandardItemModel* m_model;
        QStandardItem* m_rootItem;
        //...
        ScanArguments_Records* m_scanArguments;
        ScanResults_Records* m_scanResults;
        //...
        int m_activeThreads = 0;
        int m_endedThreads = 0;

    public:
        explicit Dns(QWidget *parent = nullptr);
        ~Dns();

    signals:
        void stop();
        void sendLog(QString log);

    public slots:
        void scanThreadEnded();
        //...
        void logs(QString log);

    private slots:
        void on_pushButton_action_clicked();
        void on_pushButton_start_clicked();
        void on_pushButton_stop_clicked();
        void on_toolButton_config_clicked();
        //...
        void on_pushButton_removeTargets_clicked();
        void on_pushButton_clearTargets_clicked();
        void on_pushButton_loadTargets_clicked();
        void on_pushButton_addTargets_clicked();
        void on_lineEdit_targets_returnPressed();
        //...
        void on_pushButton_removeSrvWordlist_clicked();
        void on_pushButton_clearSrvWordlist_clicked();
        void on_pushButton_loadSrvWordlist_clicked();
        void on_pushButton_addSrvWordlist_clicked();
        void on_lineEdit_srvWordlist_returnPressed();
        //...
        void on_pushButton_clearResults_clicked();
        void on_treeView_results_customContextMenuRequested(const QPoint &pos);
        //...
        void actionSendToSave();
        void actionSendToMultiLevel();
        void actionCollectAllRecords();
        //...
        void cursorOpenInBrowser();
        void cursorSendToSave();
        void cursorSendToMultiLevel();
};

#endif // DNS_H
