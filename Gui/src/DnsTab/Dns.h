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

    public:
        explicit Dns(QWidget *parent = nullptr);
        ~Dns();
        //...
        void showContextMenu_ActionButton(QPoint position);
        void showContextMenu_RightClick();
        //...
        int wordlistCount_dnsRecords = 0;
        //...
        scanArguments_dnsRecords *scanArguments;
        scanResults_dnsRecords *scanResults;
        //...
        QStandardItemModel *resultsRootModel;
        QStandardItem *root_item;
        //...
        int activeThreads_dnsRecords = 0;
        int endedThreads_dnsRecords = 0;

    private slots:
        void on_pushButton_remove_dnsRecords_clicked();
        void on_pushButton_clearWordlist_dnsRecords_clicked();
        void on_pushButton_load_dnsRecords_clicked();
        void on_pushButton_add_dnsRecords_clicked();
        void on_pushButton_clearOutput_dnsRecords_clicked();
        void on_pushButton_action_dnsRecords_clicked();
        void on_pushButton_start_dnsRecords_clicked();
        void on_pushButton_stop_dnsRecords_clicked();
        void on_pushButton_reload_dnsRecords_clicked();
        void on_toolButton_config_dnsRecords_clicked();
        void on_lineEdit_wordlist_dnsRecords_returnPressed();
        void on_treeView_results_dnsRecords_customContextMenuRequested(const QPoint &pos);
        // self...
        void startEnumeration_dnsRecords();
        // context menus...
        void actionSendToSave_dnsRecords();
        void actionSendToMultiLevel_dnsRecords();
        void actionCollectAllRecords_dnsRecords();
        //...
        void cursorOpenInBrowser_dnsRecords();
        void cursorSendToSave_dnsRecords();
        void cursorSendToMultiLevel_dnsRecords();


public slots:
        void onThreadEnded();
        void logs_dnsRecords(QString log);

    signals:
        void stop_dnsRecords();
        void sendLog(QString log);

    private:
        Ui::Dns *ui;
};

#endif // DNS_H
