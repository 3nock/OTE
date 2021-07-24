#ifndef SAVE_H
#define SAVE_H

#include "src/engines/Base.h"
#include "src/core.h"

namespace Ui {
    class Save;
}

class Save : public BaseClass{
    Q_OBJECT

    private:
        Ui::Save *ui;
        ResultsModel *m_resultsModel;

    public:
        explicit Save(QWidget *parent = nullptr, ResultsModel *resultsModel = nullptr);
        ~Save();
        int total_subdomain_count = 0;
        QTemporaryFile subdomains_file;
        int sort;

    signals:
        // sending status and logs to the mainWindow...
        void sendStatus(QString status);
        void sendLog(QString log);

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
};

#endif // SAVE_H
