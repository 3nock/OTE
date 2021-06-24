#ifndef BRUTE_H
#define BRUTE_H

// headers...
#include "lib-brute.h"
#include "WordlistGeneratorDialog.h"
#include "BruteConfigDialog.h"
#include "WordlistDialog.h"
#include "BruteEnumerator.h"


// structures...
struct status{
    bool isRunning = false;
    bool isStopped = false;
    bool isPaused = false;
    int numberOfScansDone = 0;
    int numberOfDifferentDomains = 0;
};
typedef struct status status;

namespace Ui {
    class Brute;
}

class Brute : public QWidget{
    Q_OBJECT

    private:
        Ui::Brute *ui;

    private:
        void checkWildcards();
        //...
        QStandardItemModel *m_model;
        //...
        QStandardItem *m_results;
        //...
        ScanArguments_Brute *m_scanArguments;
        //...
        int subdomainCount = 0;
        //...
        int wordlistCount = 0;
        //...
        int activeThreads = 0;
        int endedThreads = 0;
        //...
        QString currentPath;

    public:
        explicit Brute(QWidget *parent = nullptr);
        ~Brute();

    public slots:
        void onSendResultsToSave();

    signals:
        void stop();
        //...
        void sendStatus(QString status);
        void sendLog(QString log);
        // ...
        void sendResultsToSave(QString);
        void changeTabToSave();

    private slots:
        // for subBrute...
        void on_pushButton_add_clicked();
        void on_pushButton_clearWordlist_clicked();
        void on_pushButton_remove_clicked();
        void on_pushButton_clearResults_clicked();
        void on_pushButton_start_clicked();
        void on_pushButton_load_clicked();
        void on_pushButton_generate_clicked();
        void on_pushButton_action_clicked();
        void on_pushButton_stop_clicked();
        void on_toolButton_config_clicked();
        void on_toolButton_wordlist_clicked();
        void on_lineEdit_wordlist_returnPressed();
        void on_lineEdit_targetDomain_returnPressed();
        void on_tableView_results_customContextMenuRequested(const QPoint &pos);

        // self implemented slots...
        void actionSendToMultiLevel();
        void actionSendToSave();
        void actionSendToDnsRecords();
        //...
        void cursorSendToSave();
        void cursorSendToDnsRecords();
        void cursorOpenInBrowser();
        //...
        void startEnumeration_subBrute();
        void startEnumeration_tldBrute();
        //...
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        //...
        void onThreadEnd();
        //...
        void onWordlistFilename(QString);
        //...
        void logs(QString log);
};

#endif // BRUTE_H
