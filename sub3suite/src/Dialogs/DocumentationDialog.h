#ifndef DOCUMENTATIONDIALOG_H
#define DOCUMENTATIONDIALOG_H

#include <QDialog>
#include "src/utils/utils.h"


namespace Ui {
class DocumentationDialog;
}

class DocumentationDialog : public QDialog{
        Q_OBJECT

    public:
        DocumentationDialog(QWidget *parent = nullptr); // for general documentation
        DocumentationDialog(ENGINE engine, QWidget *parent = nullptr); // for specific documentation
        ~DocumentationDialog();

    private slots:
        void on_buttonOk_clicked();
        void on_buttonBack_clicked();

    private:
        Ui::DocumentationDialog *ui;
        void general_documentation();
        void about_osint();
        void about_raw();
        void about_brute();
        void about_active();
        void about_dns();
        void about_ssl();
};

#endif // DOCUMENTATIONDIALOG_H
