#ifndef LEVEL_H
#define LEVEL_H

#include "lib-level.h"
#include "LevelEnumerator.h"
#include "src/BruteTab/WordlistDialog.h"

namespace Ui {
    class Level;
}

class Level : public QDialog{
        Q_OBJECT

    private:
        Ui::Level *ui;

    private:
        void startEnumeration();
        //...
        QStandardItemModel* m_model_results;
        //...
        ScanArguments_level* m_scanArguments;
        status *m_status;
        //...
        int m_activeThreads = 0;
        int m_endedThreads = 0;

    public:
        explicit Level(QWidget *parent = nullptr);
        ~Level();

    signals:
        void sendStatus(QString log);
        void sendLog(QString log);
        void stop();

    public slots:
        void scanThreadEnd();
        void scanResult(QString subdomain, QString ipAddress);
        //...
        void logs(QString log);
        void onWordlistFilename(QString wordlistFilename);

    private slots:
        void on_pushButton_start_clicked();
        void on_pushButton_stop_clicked();
        void on_toolButton_config_clicked();
        void on_pushButton_clearResults_clicked();
        void on_pushButton_action_clicked();
        void on_pushButton_removeTargets_clicked();
        void on_pushButton_clearTargets_clicked();
        void on_pushButton_loadTargets_clicked();
        void on_pushButton_removeWordlist_clicked();
        void on_pushButton_clearWordlist_clicked();
        void on_pushButton_loadWordlist_clicked();
        void on_pushButton_addWordlist_clicked();
        void on_toolButton_wordlist_clicked();
        void on_lineEdit_wordlist_returnPressed();
        void on_pushButton_addTargets_clicked();
        void on_lineEdit_targets_returnPressed();
        void on_tableView_results_customContextMenuRequested(const QPoint &pos);
        //...
        void actionSendToMultiLevel();
        void actionSendToSave();
        void actionSendToDnsRecords();
        //...
        void cursorSendToSave();
        void cursorSendToDnsRecords();
        void cursorOpenInBrowser();
};

#endif // LEVEL_H
