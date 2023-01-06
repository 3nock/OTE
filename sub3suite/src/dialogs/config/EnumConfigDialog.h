#ifndef ENUMCONFIGDIALOG_H
#define ENUMCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class EnumConfigDialog;
}

class EnumConfigDialog : public QDialog{
        Q_OBJECT

    public:
        EnumConfigDialog(QWidget *parent = nullptr);
        ~EnumConfigDialog();

    private slots:
        void on_buttonOk_clicked();
        void on_buttonCancel_clicked();

    private:
        Ui::EnumConfigDialog *ui;

        void mLoadConfig();
        void mSaveConfig();
};

#endif // ENUMCONFIGDIALOG_H
