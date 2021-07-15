#ifndef SAVE_H
#define SAVE_H

#include "src/core.h"

namespace Ui {
    class Save;
}

class Save : public QDialog{
    Q_OBJECT

    public:
        explicit Save(QWidget *parent = nullptr);
        ~Save();
        int total_subdomain_count = 0;
        QTemporaryFile subdomains_file;
        int sort;

    // signals and slots...
    signals:
        // sending status and logs to the mainWindow...
        void sendStatus(QString status);
        void sendLog(QString log);
        // signals to trigger saving of the enumerated results...
        void getActiveResults();
        void getBruteResults();
        void getOsintResults();

    public slots:
        void onReceiveResults(QString);

    private slots:
        void on_pushButton_clearResults_Save_clicked();
        void on_pushButton_clear_Save_clicked();
        void on_pushButton_remove_Save_clicked();
        void on_pushButton_load_Save_clicked();
        void on_pushButton_save_Save_clicked();
        void on_pushButton_filter_Save_clicked();
        void on_pushButton_back_Save_clicked();
        void on_pushButton_sort_Save_clicked();
        void on_toolButton_get_Save_clicked();
        void on_lineEdit_filter_Save_returnPressed();

    private:
        Ui::Save *ui;
};

#endif // SAVE_H
