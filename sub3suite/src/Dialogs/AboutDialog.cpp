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

#include "src/s3s.h"
#include "src/utils/Config.h"


AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent),
    ui(new Ui::AboutDialog),
    m_authorsModel(new QStandardItemModel),
    m_modulesModel(new QStandardItemModel),
    m_foldersModel(new QStandardItemModel)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/res/icons/about.png"));

    /* github account */
    s3s_ClickableLabel *githubLabel = new s3s_ClickableLabel("", this);
    QPixmap github_logo(":/img/res/icons/github.png");
    githubLabel->setPixmap(github_logo);
    ui->horizontalLayoutGithub->insertWidget(0, githubLabel);

    connect(githubLabel, &s3s_ClickableLabel::clicked, this, [=](){
        QDesktopServices::openUrl(QUrl("https://github.com/3nock/sub3suite", QUrl::TolerantMode));
    });

    ///
    /// for sub3suite...
    ///

    /* setting up logo */
    QPixmap logo_sub3suite(":/img/res/icons/sub3suite.png");
    logo_sub3suite.scaled(ui->labelLogo->width(), ui->labelLogo->height());
    ui->labelLogo->setPixmap(logo_sub3suite);
    ui->labelLogo->setScaledContents(true);

    /* setting up version info */
    ui->labelVersion->setText("Version 0.1 (BETA)");

    /* setting up description */
    ui->textBrowserDescription->setOpenExternalLinks(true);
    ui->textBrowserDescription->append("Copyright 2020-2022 © Enock Nicholaus <a href=\"https://twitter.com/3nock_\">@3nock</a> and contributors.");

    ui->textBrowserDescription->append("\n");

    ui->textBrowserDescription->append("License GPLv3: GNU GPL version 3 or later <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">https://www.gnu.org/licenses/gpl-3.0.html</a>."
                                  " This is free software; see the source for copying conditions."
                                  " There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");

    ui->textBrowserDescription->append("\nsub3suite is Open Source Software released under the GNU General Public License.\n");

    ui->textBrowserDescription->append("visit <a href=\"https://3nock.github.io\">https://3nock.github.io</a> for more information.");

    ui->textBrowserDescription->append("\n");

    ui->textBrowserDescription->append("follow <a href=\"https://twitter.com/sub3suite\">@sub3suite</a> for more updates on the project.");

    ///
    /// for authors...
    ///

    CONFIG.beginGroup("authors");
    foreach(const QString &author, CONFIG.allKeys())
        m_authorsModel->appendRow({new QStandardItem(author),
                                   new QStandardItem(CONFIG.value(author).toString())});
    CONFIG.endGroup();
    m_authorsModel->setHorizontalHeaderLabels({" Author", " Contact"});
    ui->tableViewContributors->setModel(m_authorsModel);
    ui->tableViewContributors->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///
    /// for modules...
    ///

    CONFIG_OSINT.beginGroup("modules");
    foreach(const QString &module, CONFIG_OSINT.allKeys())
        m_modulesModel->appendRow({new QStandardItem(module),
                                   new QStandardItem(CONFIG_OSINT.value(module).toString())});
    CONFIG_OSINT.endGroup();
    m_modulesModel->setHorizontalHeaderLabels({" Module", " Source"});
    ui->tableViewModules->setModel(m_modulesModel);
    ui->tableViewModules->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///
    /// for donations...
    ///

    CONFIG.beginGroup("donors");
    foreach(const QString &donor, CONFIG.allKeys())
        m_authorsModel->appendRow({new QStandardItem(donor),
                                   new QStandardItem(CONFIG.value(donor).toString())});
    CONFIG.endGroup();
    m_foldersModel->setHorizontalHeaderLabels({" Name", " Contact"});
    ui->tableViewDonations->setModel(m_foldersModel);
    ui->tableViewDonations->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///
    /// for LICENSE...
    ///

    QFile file(":/files/res/files/LICENSE");
    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /* append LICENSE file content to the plainTextEdit */
        ui->plainTextEditLicense->setPlainText(file.readAll());
        file.close();
    }
}
AboutDialog::~AboutDialog(){
    delete m_foldersModel;
    delete m_modulesModel;
    delete m_authorsModel;
    delete ui;
}


void AboutDialog::on_tableViewModules_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->tableViewModules->selectionModel()->isSelected(ui->tableViewModules->currentIndex()))
        return;

    if(ui->tableViewModules->currentIndex().column() == 0)
        return;

    /* creating the context menu... */
    QMenu menu(this);

    menu.addAction("Copy", this, [=](){
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(ui->tableViewModules->currentIndex().data().toString());
    });
    menu.addAction("Open in browser", this, [=](){
        QDesktopServices::openUrl(QUrl(ui->tableViewModules->currentIndex().data().toString(), QUrl::TolerantMode));
    });

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

void AboutDialog::on_tableViewContributors_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->tableViewContributors->selectionModel()->isSelected(ui->tableViewContributors->currentIndex()))
        return;

    /* creating the context menu... */
    QMenu menu(this);

    menu.addAction("Copy", this, [=](){
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(ui->tableViewContributors->currentIndex().data().toString());
    });

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}

void AboutDialog::on_tableViewDonations_customContextMenuRequested(const QPoint &pos){
    Q_UNUSED(pos);

    /* check if user right clicked on items else dont show the context menu... */
    if(!ui->tableViewDonations->selectionModel()->isSelected(ui->tableViewDonations->currentIndex()))
        return;

    /* creating the context menu... */
    QMenu menu(this);

    menu.addAction("Copy", this, [=](){
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setText(ui->tableViewDonations->currentIndex().data().toString());
    });

    /* showing the context menu... */
    menu.exec(QCursor::pos());
}
