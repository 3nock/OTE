//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef VIEWS_WELCOMEVIEW_H
#define VIEWS_WELCOMEVIEW_H

#include <QWidget>

#include "src/template/Template.h"

namespace Ui {
class WelcomeView;
}

class WelcomeView : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeView(QWidget *parent = nullptr);
    ~WelcomeView();

public slots:
    void onNewTemplate(OTE::Template *tmplt);

private:
    Ui::WelcomeView *ui;
};

#endif // VIEWS_WELCOMEVIEW_H
