#ifndef LEVEL_H
#define LEVEL_H

#include "lib-level.h"
#include "src/BruteTab/WordlistDialog.h"

namespace Ui {
    class Level;
}

/*
struct scanArguments_level{
    QListWidget* wordlist;
    QStandardItemModel* model_subdomains;
    QStandardItemModel* model_results;
    QStandardItemModel* model_allSubdomains;
}; struct scanArguments_level _scanArguments_level;
*/

class Level : public QDialog{
        Q_OBJECT

    private:
        Ui::Level *ui;

    private:
        QStandardItemModel* m_model_subdomains;
        QStandardItemModel* m_model_results;
        QStandardItemModel* m_model_allResults;
        //...
        int wordlistCount = 0;
        int subdomainsCount = 0;

    public:
        explicit Level(QWidget *parent = nullptr);
        ~Level();

    public slots:
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
};

#endif // LEVEL_H
