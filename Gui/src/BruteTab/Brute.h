#ifndef BRUTE_H
#define BRUTE_H

// headers...
#include "lib-brute.h"
#include "WordlistGeneratorDialog.h"
#include "BruteConfigDialog.h"
#include "WordlistDialog.h"
#include "BruteEnumerator.h"


namespace Ui {
    class Brute;
}

class Brute : public QWidget{
    Q_OBJECT

    private:
        Ui::Brute *ui;

    private:
        void checkWildcards();
        void showContextMenu_actionButton(int enumName, QPoint position);
        void showContextMenu_rightClick(int enumName);
        //...
        QStandardItemModel *model_subBrute;
        QStandardItemModel *model_tldBrute;
        QStandardItemModel *model_activeSubdomains;
        //...
        QStandardItem *results_subBrute;
        QStandardItem *results_tldBrute;
        QStandardItem *results_activeSubdomains;
        //...
        ScanArguments_Brute scanArguments_subBrute;
        ScanArguments_Brute scanArguments_tldBrute;
        ScanArguments_Brute scanArguments_activeSubdomains;
        //...
        int subdomainCount_tldBrute = 0;
        int subdomainCount_subBrute = 0;
        int subdomainCount_activeSubdomains = 0;
        //...
        int wordlistCount_tldBrute= 0;
        int wordlistCount_subBrute = 0;
        int wordlistCount_activeSubdomains = 0;
        //...
        int activeThreads_subBrute = 0;
        int activeThreads_tldBrute = 0;
        int activeThreads_activeSubdomains = 0;
        int endedThreads_subBrute = 0;
        int endedThreads_tldBrute = 0;
        int endedThreads_activeSubdomains = 0;
        //...
        QString currentPath;

    public:
        explicit Brute(QWidget *parent = nullptr);
        ~Brute();

    public slots:
        void onSendResultsToSave();

    signals:
        void stop_subBrute();
        void stop_tldBrute();
        void stop_activeSubdomains();
        //...
        void sendStatus(QString status);
        void sendLog(QString log);
        // ...
        void sendResultsToSave(QString);
        void changeTabToSave();

    private slots:
        // for subBrute...
        void on_pushButton_add_subBrute_clicked();
        void on_pushButton_clearWordlist_subBrute_clicked();
        void on_pushButton_remove_subBrute_clicked();
        void on_pushButton_clearResults_subBrute_clicked();
        void on_pushButton_start_subBrute_clicked();
        void on_pushButton_load_subBrute_clicked();
        void on_pushButton_generate_subBrute_clicked();
        void on_pushButton_action_subBrute_clicked();
        void on_pushButton_stop_subBrute_clicked();
        void on_pushButton_reloadEnumeratedWordlist_subBrute_clicked();
        void on_toolButton_config_subBrute_clicked();
        void on_toolButton_wordlist_subBrute_clicked();
        void on_lineEdit_wordlist_subBrute_returnPressed();
        void on_lineEdit_targetDomain_subBrute_returnPressed();
        void on_tableView_results_subBrute_customContextMenuRequested(const QPoint &pos);

        //  For tldBrute...
        void on_pushButton_add_tldBrute_clicked();
        void on_pushButton_remove_tldBrute_clicked();
        void on_pushButton_clearWordlist_tldBrute_clicked();
        void on_pushButton_clearResults_tldBrute_clicked();
        void on_pushButton_start_tldBrute_clicked();
        void on_pushButton_load_tldBrute_clicked();
        void on_pushButton_stop_tldBrute_clicked();
        void on_pushButton_action_tldBrute_clicked();
        void on_pushButton_reloadEnumeratedWordlist_tldBrute_clicked();
        void on_pushButton_generate_tldBrute_clicked();
        void on_toolButton_config_tldBrute_clicked();
        void on_toolButton_wordlist_tldBrute_clicked();
        void on_lineEdit_manualWordlist_tldBrute_returnPressed();
        void on_lineEdit_targetDomain_tldBrute_returnPressed();
        void on_tableView_results_tldBrute_customContextMenuRequested(const QPoint &pos);

        // For activeSubdomains...
        void on_pushButton_reloadEnumeratedWordlist_activeSubdomains_clicked();
        void on_pushButton_get_activeSubdomains_clicked();
        void on_pushButton_remove_activeSubdomains_clicked();
        void on_pushButton_clearWordlist_activeSubdomains_clicked();
        void on_pushButton_load_activeSubdomains_clicked();
        void on_pushButton_add_activeSubdomains_clicked();
        void on_pushButton_clearResults_activeSubdomains_clicked();
        void on_pushButton_start_activeSubdomains_clicked();
        void on_pushButton_stop_activeSubdomains_clicked();
        void on_pushButton_action_activeSubdomains_clicked();
        void on_toolButton_config_activeSubdomains_clicked();
        void on_lineEdit_manualWordlist_activeSubdomains_returnPressed();
        void on_tableView_results_activeSubdomains_customContextMenuRequested(const QPoint &pos);

        // self implemented slots...
        void actionSendToMultiLevel_subBrute();
        void actionSendToMultiLevel_tldBrute();
        void actionSendToMultiLevel_activeSubdomains();
        void actionSendToSave_subBrute();
        void actionSendToSave_tldBrute();
        void actionSendToSave_activeSubdomains();
        void actionSendToDnsRecords_subBrute();
        void actionSendToDnsRecords_tldBrute();
        void actionSendToDnsRecords_activeSubdomains();
        //...
        void cursorSendToSave_subBrute();
        void cursorSendToSave_tldBrute();
        void cursorSendToSave_activeSubdomains();
        void cursorSendToDnsRecords_subBrute();
        void cursorSendToDnsRecords_tldBrute();
        void cursorSendToDnsRecords_activeSubdomains();
        void cursorOpenInBrowser_subBrute();
        void cursorOpenInBrowser_tldBrute();
        void cursorOpenInBrowser_activeSubdomains();
        //...
        void startEnumeration_subBrute();
        void startEnumeration_tldBrute();
        void startEnumeration_activeSubdomains();
        //...
        void resolvedSubdomain_subBrute(QString subdomain, QString ipAddress);
        void resolvedSubdomain_tldBrute(QString subdomain, QString ipAddress);
        void resolvedSubdomain_activeSubdomains(QString subdomain, QString ipAddress);
        //...
        void onThreadEnd_subBrute();
        void onThreadEnd_tldBrute();
        void onThreadEnd_activeSubdomains();
        //...
        void onWordlistFilename_subBrute(QString);
        void onWordlistFilename_tldBrute(QString);
        //...
        void logs_subBrute(QString log);
        void logs_tldBrute(QString log);
        void logs_activeSubdomains(QString log);
};

#endif // BRUTE_H
