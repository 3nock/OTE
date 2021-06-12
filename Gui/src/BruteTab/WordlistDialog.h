#ifndef WORDLISTDIALOG_H
#define WORDLISTDIALOG_H

#include "lib-brute.h"

namespace Ui {
    class WordListDialog;
}

class WordListDialog : public QDialog{
        Q_OBJECT

    private:
        int m_enumName;
        void m_setupSpecialWordlists();

    public:
        explicit WordListDialog(QWidget *parent = nullptr, int enumName = 0);
        ~WordListDialog();

    private slots:
        void on_pushButton_cancel_clicked();
        void on_pushButton_clear_clicked();
        void on_pushButton_remove_clicked();
        void on_pushButton_load_clicked();
        void on_pushButton_add_clicked();
        void on_pushButton_create_clicked();
        void on_pushButton_ok_clicked();
        void on_checkBox_newSpecialWordlist_clicked(bool);

    signals:
        // emits the filename of the choosen wordlist...
        void wordlistFilename(QString);

    private:
        Ui::WordListDialog *ui;
};

#endif // WORDLISTDIALOG_H
