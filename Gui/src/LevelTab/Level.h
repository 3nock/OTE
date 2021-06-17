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
        QStandardItemModel* m_model_subdomains;
        QStandardItemModel* m_model_results;
        //...
        ScanArguments_level* m_scanArguments;
        //...
        //int resultsCount = 0;
        //int wordlistCount = 0;
        //int subdomainsCount = 0;
        //...
        int activeThreads = 0;
        int endedThreads = 0;
        //...
        int level = 0;
        int doneLevels = 0;
        int startOfNewLevel = 0;

    public:
        explicit Level(QWidget *parent = nullptr);
        ~Level();
        //...
        void startEnumeration();

    public slots:
        void resolvedSubdomain(QString subdomain, QString ipAddress);
        void logs(QString log);
        void onThreadEnd();
        void onWordlistFilename(QString wordlistFilename);

    private slots:
        void on_pushButton_start_clicked();
        void on_pushButton_stop_clicked();
        void on_toolButton_config_clicked();
        void on_pushButton_clearResults_clicked();
        void on_pushButton_action_clicked();
        void on_pushButton_removeSubdomains_clicked();
        void on_pushButton_clearSubdomains_clicked();
        void on_pushButton_loadSubdomains_clicked();
        void on_pushButton_removeWordlist_clicked();
        void on_pushButton_clearWordlist_clicked();
        void on_pushButton_loadWordlist_clicked();
        void on_pushButton_addWordlist_clicked();
        void on_toolButton_wordlist_clicked();
        void on_lineEdit_wordlist_returnPressed();
        void on_pushButton_addSubdomains_clicked();
        void on_lineEdit_subdomains_returnPressed();

    signals:
        void sendStatus(QString log);
        void sendLog(QString log);
        void stop();
};

#endif // LEVEL_H
