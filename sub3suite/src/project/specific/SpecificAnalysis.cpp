/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "SpecificAnalysis.h"
#include "ui_SpecificAnalysis.h"

 /*
  * use the api:
  *         https://jsonwhois.com/
  * and     https://www.domaintools.com/resources/api-documentation/authentication/
  * and     https://team-cymru.com/community-services/ip-asn-mapping/
  *
  * and     https://ip-api.com/docs
  * and     https://toolbox.googleapps.com/apps/dig/
  */
/*
 * Screenshot:
 https://doc.qt.io/qt-5/qtwidgets-desktop-screenshot-example.html

You need to render the webpage in order to create a snapshot. To render the page in C++ you need to include a browser engine. You can do this easily using Qt (a toolkit for c++). This is taken from the documentation of Qt and is subdomainIp you need to show a webpage.

QWebView *view = new QWebView(ui->centralWidget);
view->load(QUrl("http://qt.nokia.com/"));
view->show();
The view object has a loadFinished signal. You could connect some code to this signal. When the page is rendered you tak a snapshot as described here. It boils down to:

QPixmap::grabWindow(mainwindow->winId())
When you have got the screenshot you can return the bytes on stdout from your cgi and your done.
*/
SpecificAnalysis::SpecificAnalysis(QWidget *parent) :QWidget(parent),ui(new Ui::SpecificAnalysis)
{
    ui->setupUi(this);
}
SpecificAnalysis::~SpecificAnalysis(){
    delete ui;
}
