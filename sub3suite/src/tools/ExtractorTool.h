/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#ifndef EXTRACTORTOOL_H
#define EXTRACTORTOOL_H

#include <QDialog>

namespace Ui {
class ExtractorTool;
}

class ExtractorTool : public QDialog
{
    Q_OBJECT

public:
    explicit ExtractorTool(QWidget *parent = nullptr);
    ~ExtractorTool();

private:
    Ui::ExtractorTool *ui;
};

#endif // EXTRACTORTOOL_H
