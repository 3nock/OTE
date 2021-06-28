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
        void startEnumeration();
        //...
        QStandardItemModel *m_model;
        ScanArguments_Brute *m_scanArguments;
        //...
        int activeThreads = 0;
        int endedThreads = 0;
        //...
        QString currentPath;

    public:
        explicit Brute(QWidget *parent = nullptr);
        ~Brute();

    public slots:
        void scanResult(QString subdomain, QString ipAddress);
        void scanThreadEnded();
        //...
        void logs(QString log);
        //...
        void onWordlistFilename(QString);
        //...
        void onSendResultsToSave();

    signals:
        void stop();
        void pause();
        //...
        void sendStatus(QString status);
        void sendLog(QString log);
        // ...
        void sendResultsToSave(QString);
        void changeTabToSave();

    private slots:
        void on_pushButton_start_clicked();
        void on_pushButton_pause_clicked();
        void on_pushButton_stop_clicked();
        void on_pushButton_action_clicked();
        void on_toolButton_config_clicked();
        void on_pushButton_clearResults_clicked();
        void on_pushButton_reload_clicked();
        void on_pushButton_addWordlist_clicked();
        void on_pushButton_clearWordlist_clicked();
        void on_pushButton_removeWordlist_clicked();
        void on_pushButton_loadWordlist_clicked();
        void on_pushButton_generateWordlist_clicked();
        void on_pushButton_removeTargets_clicked();
        void on_pushButton_clearTargets_clicked();
        void on_pushButton_loadTargets_clicked();
        void on_pushButton_addTargets_clicked();
        void on_toolButton_wordlist_clicked();
        void on_radioButton_tldBrute_clicked();
        void on_radioButton_subBrute_clicked();
        void on_lineEdit_wordlist_returnPressed();
        void on_lineEdit_target_returnPressed();
        void on_lineEdit_multipleTargets_returnPressed();
        void on_tableView_results_customContextMenuRequested(const QPoint &pos);
        void on_comboBox_target_currentIndexChanged(int index);
        // ...
        void actionSendToMultiLevel();
        void actionSendToSave();
        void actionSendToDnsRecords();
        //...
        void cursorSendToSave();
        void cursorSendToDnsRecords();
        void cursorOpenInBrowser();
};

#endif // BRUTE_H
