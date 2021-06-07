#ifndef DNS_H
#define DNS_H

// headers...
#include "src/core.h"
#include "src/BruteTab/BruteConfigDialog.h"

namespace Ui {
    class Dns;
}

class Dns : public QDialog{
    Q_OBJECT

    public:
        explicit Dns(QWidget *parent = nullptr);
        int wordlistCount_dnsRecords = 0;
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
        void on_pushButton_reload_dnsRecords_clicked();
        void on_toolButton_config_dnsRecords_clicked();

    private:
        Ui::Dns *ui;
};

#endif // DNS_H
