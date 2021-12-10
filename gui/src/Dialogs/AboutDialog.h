#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>


namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog{
        Q_OBJECT

    public:
        explicit AboutDialog(QWidget *parent = nullptr);
        ~AboutDialog();

    private slots:
        void on_buttonCopy_clicked();

    private:
        Ui::AboutDialog *ui;
        QStandardItemModel *m_authorsModel;
        QStandardItemModel *m_modulesModel;
        QStandardItemModel *m_foldersModel;
        void m_setAuthors();
        void m_setModules();
        void m_setFolders();
};

#endif // ABOUTDIALOG_H
