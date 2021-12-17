#ifndef ACTIVECONFIGDIALOG_H
#define ACTIVECONFIGDIALOG_H

#include <QDialog>
#include <QStringListModel>


namespace Ui {
class ActiveConfigDialog;
}

class ActiveConfigDialog : public QDialog{
        Q_OBJECT

    public:
        explicit ActiveConfigDialog(QWidget *parent = nullptr);
        ~ActiveConfigDialog();

    private slots:
        void on_radioButtonCustomNameservers_clicked();
        void on_radioButtonRandomNameservers_clicked();
        void on_radioButtonSingleNameserver_clicked();
        void on_checkBoxNoDuplicates_clicked(bool checked);
        void on_checkBoxAutosave_clicked(bool checked);

    private:
        Ui::ActiveConfigDialog *ui;
        QStringListModel *m_customNameserverListModel;
};

#endif // ACTIVECONFIGDIALOG_H
