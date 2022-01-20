#ifndef STARTUPDIALOG_H
#define STARTUPDIALOG_H

#include <QDialog>


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
        void on_checkBoxTemporary_clicked(bool checked);
        void on_groupBoxNewProject_toggled(bool arg1);
        void on_groupBoxExistingProject_toggled(bool arg1);

    private:
        Ui::StartupDialog *ui;
        QString *m_project;
};

#endif // STARTUPDIALOG_H
