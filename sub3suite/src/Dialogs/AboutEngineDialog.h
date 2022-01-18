#ifndef ABOUTENGINEDIALOG_H
#define ABOUTENGINEDIALOG_H

#include <QDialog>
#include "src/utils/utils.h"


namespace Ui {
class AboutEngineDialog;
}

class AboutEngineDialog : public QDialog{
        Q_OBJECT

    public:
        AboutEngineDialog(ENGINE engine, QWidget *parent = nullptr);
        ~AboutEngineDialog();

private slots:
        void on_buttonOk_clicked();

private:
        Ui::AboutEngineDialog *ui;
        void about_osint();
        void about_raw();
        void about_brute();
        void about_active();
        void about_dns();
        void about_ssl();
        void about_ip();
};

#endif // ABOUTENGINEDIALOG_H
