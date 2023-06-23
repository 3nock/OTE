//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExplorerSitemapView.h"
#include "ui_ExplorerSitemapView.h"

#include <QDomDocument>
#include <QJsonDocument>
#include <QMenu>

#include "src/core/Database.h"

ExplorerSitemapView::ExplorerSitemapView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExplorerSitemapView),
    mModelSitemap(new QStandardItemModel)
{
    ui->setupUi(this);

    ui->splitter->setSizes(QList<int>() << static_cast<int>((this->width() * 0.40))
                                        << static_cast<int>((this->width() * 0.60)));

    ui->plainTextEditResult->setPlaceholderText(tr("API endpoint's query result"));

    mModelSitemap->setHorizontalHeaderLabels({tr("Template"), tr("API Endpoint"), tr("Target")});
    ui->tableViewSitemap->setModel(mModelSitemap);

    ui->plainTextEditResult->setLineWrapMode(QPlainTextEdit::NoWrap);
    mHighlighter = new OTE::SyntaxHighlighter(ui->plainTextEditResult->document());
}

ExplorerSitemapView::~ExplorerSitemapView()
{
    delete mHighlighter;
    delete mModelSitemap;
    delete ui;
}

void ExplorerSitemapView::onClear()
{
    mModelSitemap->clear();
    ui->plainTextEditResult->clear();

    mModelSitemap->setHorizontalHeaderLabels({tr("Template"), tr("API Endpoint"), tr("Target")});
}

void ExplorerSitemapView::onNewProject()
{
    auto results = OTE::Database::loadExplorerResults();
    foreach(const OTE::ExplorerResult &result, results)
    {
        OTE::Explorer::ResultItem *item = new OTE::Explorer::ResultItem(result.tmplt,
                                                                        result.endpoint,
                                                                        result.target);

        mModelSitemap->appendRow({item, item->endpointItem, item->targetItem});
    }
}

void ExplorerSitemapView::onResult(const QString &tmplt, const QString &endPoint, const QString &target)
{
    OTE::Explorer::ResultItem *item = new OTE::Explorer::ResultItem(tmplt,endPoint,target);
    mModelSitemap->appendRow({item,
                              item->endpointItem,
                              item->targetItem});
}

void ExplorerSitemapView::on_tableViewSitemap_clicked(const QModelIndex &index)
{
    ui->plainTextEditResult->clear();

    OTE::Explorer::ResultItem *item = nullptr;

    if(index.column())
        item = static_cast<OTE::Explorer::ResultItem*>(mModelSitemap->itemFromIndex(index.siblingAtColumn(0)));
    else
        item = static_cast<OTE::Explorer::ResultItem*>(mModelSitemap->itemFromIndex(index));

    QByteArray results = OTE::Database::getExplorerResult(item->text(),
                                                          item->endpointItem->text(),
                                                          item->targetItem->text()).trimmed();

    if(results.startsWith("<"))
    {
        QDomDocument doc;
        if(doc.setContent(results))
        {
            mHighlighter->setCurrentLanguage(OTE::SyntaxHighlighter::CodeXML);
            ui->plainTextEditResult->setPlainText(doc.toString(4));
        }
        else
        {
            mHighlighter->setCurrentLanguage(OTE::SyntaxHighlighter::CodeTXT);
            ui->plainTextEditResult->setPlainText(results);
        }
    }
    else if(results.startsWith("{") || results.startsWith("["))
    {
        QJsonDocument doc = QJsonDocument::fromJson(results);
        if(doc.isArray() || doc.isObject())
        {
            mHighlighter->setCurrentLanguage(OTE::SyntaxHighlighter::CodeJSON);
            ui->plainTextEditResult->setPlainText(doc.toJson());
        }
        else
        {
            mHighlighter->setCurrentLanguage(OTE::SyntaxHighlighter::CodeTXT);
            ui->plainTextEditResult->setPlainText(results);
        }
    }
    else
    {
        mHighlighter->setCurrentLanguage(OTE::SyntaxHighlighter::CodeTXT);
        ui->plainTextEditResult->setPlainText(results);
    }
}

void ExplorerSitemapView::on_tableViewSitemap_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)

    QModelIndex index = ui->tableViewSitemap->currentIndex();
    if(!index.isValid())
        return;

    OTE::Explorer::ResultItem *item;

    if(index.column())
        item = static_cast<OTE::Explorer::ResultItem*>(mModelSitemap->itemFromIndex(index.siblingAtColumn(0)));
    else
        item = static_cast<OTE::Explorer::ResultItem*>(mModelSitemap->itemFromIndex(index));

    QMenu menu(this);

    menu.addAction(tr("Remove"), this, [=]
    {
        if(OTE::Database::removeExplorerResult(item->text(),
                                               item->endpointItem->text(),
                                               item->targetItem->text()))
        {
            mModelSitemap->removeRow(index.row());
        }
    })->setIcon(QIcon(""));

    menu.exec(QCursor::pos());
}
