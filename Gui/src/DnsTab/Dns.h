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
        QStandardItemModel* m_model;
        QStandardItem* m_rootItem;
        //...
        scanArguments_dnsRecords* m_scanArguments;
        scanResults_dnsRecords* m_scanResults;
        //...
        int activeThreads = 0;
        int endedThreads = 0;
        //...
        int wordlistCount = 0;

    public:
        explicit Dns(QWidget *parent = nullptr);
        ~Dns();

    private slots:
        void on_pushButton_remove_clicked();
        void on_pushButton_clearWordlist_clicked();
        void on_pushButton_load_clicked();
        void on_pushButton_add_clicked();
        void on_pushButton_clearResults_clicked();
        void on_pushButton_action_clicked();
        void on_pushButton_start_clicked();
        void on_pushButton_stop_clicked();
        void on_toolButton_config_clicked();
        void on_lineEdit_wordlist_returnPressed();
        void on_treeView_results_customContextMenuRequested(const QPoint &pos);

        // self...
        void startEnumeration();
        // context menus...
        void actionSendToSave();
        void actionSendToMultiLevel();
        void actionCollectAllRecords();
        //...
        void cursorOpenInBrowser();
        void cursorSendToSave();
        void cursorSendToMultiLevel();

    public slots:
        void onThreadEnded();
        //...
        void logs(QString log);

    signals:
        void stop();
        void sendLog(QString log);
};

#endif // DNS_H
