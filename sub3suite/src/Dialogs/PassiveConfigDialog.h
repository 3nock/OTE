#ifndef PASSIVECONFIGDIALOG_H
#define PASSIVECONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class PassiveConfigDialog;
}

class PassiveConfigDialog : public QDialog{
        Q_OBJECT

    public:
        explicit PassiveConfigDialog(QWidget *parent = nullptr);
        ~PassiveConfigDialog();

    private slots:
        void on_buttonOk_clicked();

        void on_checkBoxNoDuplicates_clicked(bool checked);

private:
        Ui::PassiveConfigDialog *ui;
};

#endif // PASSIVECONFIGDIALOG_H
