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
    m_foldersModel(new QStandardItemModel)
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
    ui->labelVersion->setText("Version 1.0.0 (BETA)");

    /* setting up description */
    ui->textBrowserDescription->setOpenExternalLinks(true);
    ui->textBrowserDescription->append("Copyright 2020-2022 © Enock Nicholaus <a href=\"https://twitter.com/3nock_\">@3nock_</a> and contributors.");

    ui->textBrowserDescription->append("\n");

    ui->textBrowserDescription->append("License GPLv3: GNU GPL version 3 <a href=\"https://www.gnu.org/licenses/gpl-3.0.html\">https://www.gnu.org/licenses/gpl-3.0.html</a>."
                                       " This is free software; see the source for copying conditions."
                                       " There is NO warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.");

    ui->textBrowserDescription->append("\nsub3suite is Open Source Software released under the GNU General Public License.\n");

    ui->textBrowserDescription->append("visit <a href=\"https://3nock.github.io/sub3suite\">https://3nock.github.io</a> for more information.");

    ui->textBrowserDescription->append("\n");

    ui->textBrowserDescription->append("follow <a href=\"https://twitter.com/intent/follow?screen_name=sub3suite&tw_p=followbutton\">@sub3suite</a> for more updates on the project.");

    ///
    /// for authors...
    ///

    CONFIG.beginGroup(CFG_GRP_AUTHORS);
    foreach(const QString &author, CONFIG.allKeys())
        m_authorsModel->appendRow({new QStandardItem(author),
                                   new QStandardItem(CONFIG.value(author).toString())});
    CONFIG.endGroup();
    m_authorsModel->setHorizontalHeaderLabels({tr(" Author"), tr(" Contact")});
    ui->tableViewContributors->setModel(m_authorsModel);
    ui->tableViewContributors->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///
    /// for modules...
    ///

    CONFIG.beginGroup(CFG_GRP_MODULES);
    foreach(const QString &module, CONFIG.allKeys())
        m_modulesModel->appendRow({new QStandardItem(module),
                                   new QStandardItem(CONFIG.value(module).toString())});
    CONFIG.endGroup();
    m_modulesModel->setHorizontalHeaderLabels({tr(" Module"), tr(" Source")});
    ui->tableViewModules->setModel(m_modulesModel);
    ui->tableViewModules->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///
    /// for donations...
    ///

    CONFIG.beginGroup(CFG_GRP_DONORS);
    foreach(const QString &donor, CONFIG.allKeys())
        m_authorsModel->appendRow({new QStandardItem(donor),
                                   new QStandardItem(CONFIG.value(donor).toString())});
    CONFIG.endGroup();
    m_foldersModel->setHorizontalHeaderLabels({tr(" Name"), tr(" Contact")});
    ui->tableViewDonations->setModel(m_foldersModel);
    ui->tableViewDonations->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    ///
    /// version history...
    ///

    QFile file_history(":/files/res/files/HISTORY");
    if(file_history.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /* append HISTORY file content to the plainTextEdit */
        ui->plainTextEditHistory->setPlainText(file_history.readAll());
        file_history.close();
    }

    ///
    /// for LICENSE...
    ///

    QFile file_license(":/files/res/files/LICENSE");
    if(file_license.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        /* append LICENSE file content to the plainTextEdit */
        ui->plainTextEditLicense->setPlainText(file_license.readAll());
        file_license.close();
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

void AboutDialog::on_buttonCopy_clicked(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->textBrowserDescription->toPlainText());
}
