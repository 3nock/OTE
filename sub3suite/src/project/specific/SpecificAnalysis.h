/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef SPECIFICANALYSIS_H
#define SPECIFICANALYSIS_H

#include <QWidget>

namespace Ui {
    class SpecificAnalysis;
}

class SpecificAnalysis : public QWidget{
        Q_OBJECT

    public:
        explicit SpecificAnalysis(QWidget *parent = nullptr);
        ~SpecificAnalysis();

    private:
        Ui::SpecificAnalysis *ui;
};

#endif // SPECIFICANALYSIS_H
