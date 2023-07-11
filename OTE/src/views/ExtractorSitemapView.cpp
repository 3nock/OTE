//  Copyright (C) 2023 OSINT Template Engine (https://github.com/3nock/OTE). All Rights Reserved.
//
//  Licensed under the GPL License, Version 3.0 (the "License");
//  you may not use this file except in compliance with the License.
//
//  Author: Enock N. Michael (enock.n.michael@gmail.com)

#include "ExtractorSitemapView.h"
#include "ui_ExtractorSitemapView.h"

#include <QClipboard>
#include <QDebug>
#include <QFileDialog>
#include <QMenu>

#include "src/core/Config.h"
#include "src/core/Database.h"
#include "src/template/Template.h"

ExtractorSitemapView::ExtractorSitemapView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExtractorSitemapView),
    mSitemapModel(new OTE::SitemapModel::Extractor)
{
    ui->setupUi(this);

    ui->lineEditFilter->setPlaceholderText(tr("Filter..."));

    ui->tableViewSitemap->setModel(mSitemapModel->proxy);
    ui->tableViewSitemap->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    ui->comboBoxType->addItems(OTE::gInputOutput.outputTypes);
    ui->comboBoxType->setCurrentText("asn");
}

ExtractorSitemapView::~ExtractorSitemapView()
{
    delete mSitemapModel;
    delete ui;
}

void ExtractorSitemapView::onClear()
{
    ui->lineEditFilter->clear();
    mSitemapModel->clear();
    ui->labelCount->clear();
}

void ExtractorSitemapView::onNewProject()
{
    QMap<QString,QStringList> results = OTE::Database::loadExtractorResults();
    foreach(const QString &type, results.keys())
    {
        QStandardItemModel *model = mSitemapModel->getModel(type);
        foreach(const QString &result, results[type])
            model->appendRow(new QStandardItem(result));
    }

    ui->labelCount->setNum(mSitemapModel->proxy->rowCount());
}

void ExtractorSitemapView::onResult(const QString &type, const QStringList &results)
{
    QStandardItemModel *model = mSitemapModel->getModel(type);

    foreach(const QString &result, results)
        model->appendRow(new QStandardItem(result));

    ui->labelCount->setNum(mSitemapModel->proxy->rowCount());
}

void ExtractorSitemapView::onOutputTypeChanged(const QString &outputType)
{
    if(ui->comboBoxType->currentText() == outputType)
        return;

    ui->comboBoxType->setCurrentText(outputType);
}

void ExtractorSitemapView::on_buttonActions_clicked()
{
    // check if there are results available else dont show the context menu
    if(mSitemapModel->proxy->rowCount() < 1)
        return;

    // getting the position of the action button to place the context menu and
    // showing the context menu right by the side of the action button...
    QPoint pos = ui->buttonActions->mapToGlobal(QPoint(0,0));
    pos = QPoint(pos.x()+ui->buttonActions->width(), pos.y());

    // Main menu
    QMenu menu(this);

    menu.addAction(tr("Remove All"), this, [=]
    {
        while(mSitemapModel->proxy->rowCount())
        {
            QModelIndex proxyIndex = mSitemapModel->proxy->index(0, 0);
            QModelIndex index = mSitemapModel->proxy->mapToSource(proxyIndex);
            QStandardItem *item = mSitemapModel->currentModel->itemFromIndex(index);

            if(OTE::Database::removeExtractorResult(mSitemapModel->currentModelName, item->text()))
                mSitemapModel->currentModel->removeRow(index.row());
        }
        ui->labelCount->setNum(mSitemapModel->proxy->rowCount());
    })->setIcon(QIcon(":/icons/res/icons/delete.png"));

    menu.addSeparator();

    menu.addAction(tr("Copy All"), this, [=]
    {
        QString results;
        for(int i = 0; i < mSitemapModel->proxy->rowCount(); i++)
        {
            QModelIndex proxyIndex = mSitemapModel->proxy->index(i, 0);
            QModelIndex index = mSitemapModel->proxy->mapToSource(proxyIndex);
            QStandardItem *item = mSitemapModel->currentModel->itemFromIndex(index);

            results.append(item->text());
            results.append("\n");
        }
        qApp->clipboard()->setText(results);
    })->setIcon(QIcon(":/icons/res/icons/copy.png"));

    menu.addAction(tr("Save All"), this, [=]
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Filename"), OTE::gConfig.general.recentDirectory, "*");
        if(filename.isEmpty())
            return;

        QFile file(filename);
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream ts(&file);

            for(int i = 0; i < mSitemapModel->proxy->rowCount(); i++)
            {
                QModelIndex proxyIndex = mSitemapModel->proxy->index(i, 0);
                QModelIndex index = mSitemapModel->proxy->mapToSource(proxyIndex);
                QStandardItem *item = mSitemapModel->currentModel->itemFromIndex(index);

                ts << item->text();
                ts << "\n";
            }
            file.close();
        }
    })->setIcon(QIcon(":/icons/res/icons/save.png"));

    // show context menu
    menu.exec(pos);
}

void ExtractorSitemapView::on_tableViewSitemap_customContextMenuRequested(const QPoint &pos)
{
    Q_UNUSED(pos)

    QItemSelectionModel *selectionModel = ui->tableViewSitemap->selectionModel();

    // check if user right clicked on items else dont show the context menu...
    if(!selectionModel->isSelected(ui->tableViewSitemap->currentIndex()))
        return;

    QModelIndex proxyIndex = ui->tableViewSitemap->currentIndex();
    QModelIndex index = mSitemapModel->proxy->mapToSource(proxyIndex);
    QStandardItem *item = mSitemapModel->currentModel->itemFromIndex(index);

    // main menu
    QMenu menu(this);

    menu.addAction(tr("Remove"), this, [=]
    {
        if(OTE::Database::removeExtractorResult(mSitemapModel->currentModelName, item->text()))
            mSitemapModel->currentModel->removeRow(index.row());
        ui->labelCount->setNum(mSitemapModel->proxy->rowCount());
    })->setIcon(QIcon(":/icons/res/icons/delete.png"));

    menu.addSeparator();

    menu.addAction(tr("Copy"), this, [=]
    {
        qApp->clipboard()->setText(item->text());
    })->setIcon(QIcon(":/icons/res/icons/copy.png"));

    menu.addAction(tr("Save"), this, [=]
    {
        QString filename = QFileDialog::getSaveFileName(this, tr("Filename"), OTE::gConfig.general.recentDirectory, "*");
        if(filename.isEmpty())
            return;

        QFile file(filename);
        if(file.open(QIODevice::WriteOnly))
        {
            file.write(item->text().toUtf8());
            file.close();
        }
    })->setIcon(QIcon(":/icons/res/icons/save.png"));

    // show context menu
    menu.exec(QCursor::pos());
}

void ExtractorSitemapView::on_comboBoxType_currentTextChanged(const QString &type)
{
    mSitemapModel->setCurrentModel(type);
    ui->labelCount->setNum(mSitemapModel->proxy->rowCount());

    emit outputTypeChanged(type);
}

void ExtractorSitemapView::on_lineEditFilter_textChanged(const QString &pattern)
{
    mSitemapModel->proxy->setFilterFixedString(pattern);
    ui->labelCount->setNum(mSitemapModel->proxy->rowCount());
}
