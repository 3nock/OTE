//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef WIDGETS_USERKEYWIDGET_H
#define WIDGETS_USERKEYWIDGET_H

#include <QWidget>

#include "src/template/Template.h"

namespace Ui {
class UserKeyWidget;
}

class UserKeyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserKeyWidget(OTE::Template *tmplt, QWidget *parent = nullptr);
    ~UserKeyWidget();

public slots:
    void onSave();

private slots:
    void on_buttonGet_clicked();

private:
    Ui::UserKeyWidget *ui;

    OTE::Template *mTemplate;
};

#endif // WIDGETS_USERKEYWIDGET_H
