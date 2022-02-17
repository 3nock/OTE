/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

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
        void on_checkBoxNewCustomWordlist_clicked(bool checked);
        /* for choose */
        void on_buttonLoadChoosen_clicked();
        void on_buttonCreate_clicked();
        /* for generate */
        void on_buttonGenerate_clicked();

        void on_comboBoxSubstituteFrom_currentIndexChanged(int index);

        void on_comboBoxSubstituteTo_currentIndexChanged(int index);

private:
        Ui::WordListDialog *ui;
        QStringListModel *m_wordlistModel;

        /* for choose */
        void m_initChoose();
        QStringListModel *m_listModel_choose;
        QStringListModel *m_listModel_generate;
        QString m_defaultWordlist;
        QString m_specialWordlist;
        bool TLD = false;
        bool Subdomain = false;

        /* for generate */
        void m_initGenerate();
        void m_generateAZ();
        void m_generateNumbers();
        void m_generateDate();
        void m_generateSubstitutions();
};

#endif // WORDLISTDIALOG_H
