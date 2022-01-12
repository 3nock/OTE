/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef GENERALANALYSIS_H
#define GENERALANALYSIS_H

#include <QWidget>

namespace Ui {
    class GeneralAnalysis;
}

class GeneralAnalysis : public QWidget{
        Q_OBJECT

    public:
        explicit GeneralAnalysis(QWidget *parent = nullptr);
        ~GeneralAnalysis();

    private:
        Ui::GeneralAnalysis *ui;
};

#endif // GENERALANALYSIS_H
