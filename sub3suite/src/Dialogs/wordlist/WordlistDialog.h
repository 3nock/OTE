#ifndef WORDLISTDIALOG_H
#define WORDLISTDIALOG_H

#include <QDialog>
#include <QStringListModel>
#include "src/utils/utils.h"


namespace Ui {
    class WordListDialog;
}

class WordListDialog : public QDialog{
        Q_OBJECT

    public:
        WordListDialog(QWidget *parent, ENGINE engine);
        ~WordListDialog();

    private slots:
        void on_buttonCancel_clicked();
        void on_buttonCreate_clicked();
        void on_buttonOk_clicked();
        void on_checkBoxCreateNew_clicked(bool checked);

    signals:
        void choosenWordlist(QString);

    private:
        Ui::WordListDialog *ui;
        const ENGINE m_engine;
        QStringListModel *m_customWordlistModel;
        void loadWordlists();
        /* ... */
        QString m_defaultWordlist;
        QString m_specialWordlist;
};

#endif // WORDLISTDIALOG_H
