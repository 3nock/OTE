/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QFile>
#include <QMenu>
#include <QClipboard>
#include <QDesktopServices>

#include "src/utils/s3s.h"
#include "src/utils/Config.h"


AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::AboutDialog),
    m_authorsModel(new QStandardItemModel),
    m_modulesModel(new QStandardItemModel),
    m_foldersModel(new QStandardItemModel),
    m_sponsorsModel(new QStandardItemModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/about.png"));

    ///
    /// for sub3suite...
    ///

    /* setting up logo */
    QPixmap logo_sub3suite(":/img/res/icons/sub3suite.png");
    logo_sub3suite.scaled(ui->labelLogo->width(), ui->labelLogo->height());
    ui->labelLogo->setPixmap(logo_sub3suite);
    ui->labelLogo->setScaledContents(true);

    /* setting up version info */
    ui->labelVersion->setText("Version 0.0.4 (BETA)");

    /* setting up description */
    ui->textBrowserDescription->setOpenExternalLinks(true);
    QFile file_about(":/files/res/files/ABOUT");
    if(file_about.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->textBrowserDescription->append(file_about.readAll());
        file_about.close();
    }

    ///
    /// for authors...
    ///

    foreach(const QString &author, AUTHORS.allKeys())
        m_authorsModel->appendRow({new QStandardItem(author),
                                   new QStandardItem(AUTHORS.value(author).toString())});

    m_authorsModel->setHorizontalHeaderLabels({tr(" Author"), tr(" Contact")});
    ui->tableViewContributors->setModel(m_authorsModel);
    ui->tableViewContributors->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///
    /// for modules...
    ///

    foreach(const QString &module, MODULES.allKeys())
        m_modulesModel->appendRow({new QStandardItem(module),
                                   new QStandardItem(MODULES.value(module).toString())});

    m_modulesModel->setHorizontalHeaderLabels({tr(" Module"), tr(" Source")});
    ui->tableViewModules->setModel(m_modulesModel);
    ui->tableViewModules->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///
    /// for sponsors...
    ///

    foreach(const QString &donor, DONORS.allKeys())
        m_sponsorsModel->appendRow({new QStandardItem(donor),
                                   new QStandardItem(DONORS.value(donor).toString())});

    m_sponsorsModel->setHorizontalHeaderLabels({tr(" Name"), tr(" Contact")});
    ui->tableViewDonations->setModel(m_sponsorsModel);
    ui->tableViewDonations->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///
    /// version history...
    ///

    QFile file_history(":/files/res/files/HISTORY");
    if(file_history.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->plainTextEditHistory->setPlainText(file_history.readAll());
        file_history.close();
    }

    ///
    /// for LICENSE...
    ///

    QFile file_license(":/files/res/files/LICENSE");
    if(file_license.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        ui->plainTextEditLicense->setPlainText(file_license.readAll());
        file_license.close();
    }
}
AboutDialog::~AboutDialog(){
    delete m_sponsorsModel;
    delete m_foldersModel;
    delete m_modulesModel;
    delete m_authorsModel;
    delete ui;
}


void AboutDialog::on_tableViewModules_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu */
    if(!ui->tableViewModules->selectionModel()->isSelected(ui->tableViewModules->currentIndex()))
        return;

    if(ui->tableViewModules->currentIndex().column() == 0)
        return;

    /* creating the context menu */
    QMenu menu(this);

    menu.addAction("Copy", this, [=](){
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(ui->tableViewModules->currentIndex().data().toString());
    });
    menu.addAction("Open in browser", this, [=](){
        QDesktopServices::openUrl(QUrl(ui->tableViewModules->currentIndex().data().toString(), QUrl::TolerantMode));
    });

    /* showing the context menu */
    menu.exec(QCursor::pos());
}

void AboutDialog::on_tableViewContributors_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu */
    if(!ui->tableViewContributors->selectionModel()->isSelected(ui->tableViewContributors->currentIndex()))
        return;

    /* creating the context menu */
    QMenu menu(this);

    menu.addAction("Copy", this, [=](){
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(ui->tableViewContributors->currentIndex().data().toString());
    });

    /* showing the context menu */
    menu.exec(QCursor::pos());
}

void AboutDialog::on_tableViewDonations_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu */
    if(!ui->tableViewDonations->selectionModel()->isSelected(ui->tableViewDonations->currentIndex()))
        return;

    /* creating the context menu */
    QMenu menu(this);

    menu.addAction("Copy", this, [=](){
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(ui->tableViewDonations->currentIndex().data().toString());
    });

    /* showing the context menu */
    menu.exec(QCursor::pos());
}

void AboutDialog::on_buttonCopy_clicked(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->textBrowserDescription->toPlainText());
}
