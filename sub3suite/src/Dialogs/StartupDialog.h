#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QDialog>
#include <QStandardItemModel>


namespace Ui {
class StartupDialog;
}

class StartupDialog : public QDialog{
        Q_OBJECT

    public:
        explicit StartupDialog(QString *project, QWidget *parent = nullptr);
        ~StartupDialog();

    private slots:
        void on_buttonOpen_clicked();
        void on_buttonCancel_clicked();

    private:
        Ui::StartupDialog *ui;
        QString *m_project;
        QStandardItemModel *existing_model;

};

#endif // STARTUPDIALOG_H
