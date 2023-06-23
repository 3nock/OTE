//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#ifndef VIEWS_EXTRACTORSITEMAPVIEW_H
#define VIEWS_EXTRACTORSITEMAPVIEW_H

#include <QStandardItemModel>
#include <QWidget>

#include "src/models/ExtractorSitemapModel.h"

namespace Ui {
class ExtractorSitemapView;
}

class ExtractorSitemapView : public QWidget
{
    Q_OBJECT

public:
    explicit ExtractorSitemapView(QWidget *parent = nullptr);
    ~ExtractorSitemapView();

signals:
    void outputTypeChanged(const QString &outputType);

public slots:
    void onClear();
    void onNewProject();

    void onResult(const QString &type,
                  const QStringList &results);

    void onOutputTypeChanged(const QString &outputType);

private slots:
    void on_buttonActions_clicked();
    void on_tableViewSitemap_customContextMenuRequested(const QPoint &pos);
    void on_comboBoxType_currentTextChanged(const QString &arg1);
    void on_lineEditFilter_textChanged(const QString &arg1);

private:
    Ui::ExtractorSitemapView *ui;

    OTE::SitemapModel::Extractor *mSitemapModel;
};

#endif // VIEWS_EXTRACTORSITEMAPVIEW_H
