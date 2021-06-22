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
        void showContextMenu_ActionButton(QPoint position);
        void showContextMenu_RightClick();
        //...
        QStandardItemModel* model_dnsRecords;
        QStandardItemModel* model_lookup;
        QStandardItem* rootItem_dnsRecords;
        QStandardItem* results_lookup;
        //...
        scanArguments_dnsRecords* m_scanArguments_dnsRecords;
        scanArguments_lookup* m_scanArguments_lookup;
        scanResults_dnsRecords* m_scanResults_dnsRecords;
        scanResults_lookup* m_scanResults_lookup;
        //...
        int activeThreads_dnsRecords = 0;
        int activeThreads_lookup = 0;
        int endedThreads_dnsRecords = 0;
        int endedThreads_lookup = 0;
        //...
        int wordlistCount_dnsRecords = 0;
        int wordlistCount_lookup = 0;

    public:
        explicit Dns(QWidget *parent = nullptr);
        ~Dns();

    private slots:
        void on_pushButton_remove_dnsRecords_clicked();
        void on_pushButton_clearWordlist_dnsRecords_clicked();
        void on_pushButton_load_dnsRecords_clicked();
        void on_pushButton_add_dnsRecords_clicked();
        void on_pushButton_clearOutput_dnsRecords_clicked();
        void on_pushButton_action_dnsRecords_clicked();
        void on_pushButton_start_dnsRecords_clicked();
        void on_pushButton_stop_dnsRecords_clicked();
        void on_toolButton_config_dnsRecords_clicked();
        void on_lineEdit_wordlist_dnsRecords_returnPressed();
        void on_treeView_results_dnsRecords_customContextMenuRequested(const QPoint &pos);

        // for resolver...
        void on_pushButton_start_lookup_clicked();
        void on_pushButton_stop_lookup_clicked();
        void on_pushButton_clearResults_lookup_clicked();
        void on_pushButton_action_lookup_clicked();
        void on_pushButton_clearWordlist_lookup_clicked();
        void on_pushButton_removeWordlist_lookup_clicked();
        void on_pushButton_loadWordlist_lookup_clicked();
        void on_pushButton_addWordlist_lookup_clicked();
        void on_toolButton_config_lookup_clicked();
        void on_lineEdit_wordlist_lookup_returnPressed();
        void on_tableView_results_lookup_customContextMenuRequested(const QPoint &pos);

        // self...
        void startEnumeration_dnsRecords();
        void startEnumeration_lookup();
        // context menus...
        void actionSendToSave_dnsRecords();
        void actionSendToSave_lookup();
        void actionSendToMultiLevel_dnsRecords();
        void actionSendToMultiLevel_lookup();
        void actionCollectAllRecords_dnsRecords();
        void actionCollectAllRecords_lookup();
        //...
        void cursorOpenInBrowser_dnsRecords();
        void cursorOpenInBrowser_lookup();
        void cursorSendToSave_dnsRecords();
        void cursorSendToSave_lookup();
        void cursorSendToMultiLevel_dnsRecords();
        void cursorSendToMultiLevel_lookup();

    public slots:
        void onThreadEnded_dnsRecords();
        void onThreadEnded_lookup();
        //...
        void logs_dnsRecords(QString log);
        void logs_lookup(QString log);

    signals:
        void stop_dnsRecords();
        void stop_lookup();
        void sendLog(QString log);
};

#endif // DNS_H
