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
        WordListDialog(QWidget *parent, QStringListModel *wordlistModel);
        ~WordListDialog();

        void initTLDWordlist();
        void initSubdomainWordlist();

    signals:
        void wordlistLoaded();

    private slots:
        void on_tabWidget_currentChanged(int index);
        /* for choose */
        void on_buttonLoadChoosen_clicked();
        void on_buttonCreate_clicked();
        /* for generate */
        void on_buttonGenerate_clicked();

    private:
        Ui::WordListDialog *ui;
        QStringListModel *m_wordlistModel;
        /* for choose */
        void m_initChoose();
        QStringListModel *m_customWordlistModel;
        QString m_defaultWordlist;
        QString m_specialWordlist;
        bool TLD = false;
        bool Subdomain = false;
        /* for generate */
        void m_initGenerate();
        QStringListModel *m_generateWordlistModel;
};

#endif // WORDLISTDIALOG_H
