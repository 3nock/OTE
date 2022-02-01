/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

#include <QDialog>


namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QDialog{
        Q_OBJECT

    public:
        explicit PreferenceDialog(QWidget *parent = nullptr);
        ~PreferenceDialog();

    private:
        Ui::PreferenceDialog *ui;
};

#endif // PREFERENCEDIALOG_H
