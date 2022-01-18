/*
 Copyright 2020-2022 Enock Nicholaus <3nock@protonmail.com>. All rights reserved.
 Use of this source code is governed by GPL-3.0 LICENSE that can be found in the LICENSE file.

 @brief :
*/

#include "AboutDialog.h"
#include "ui_AboutDialog.h"

#include <QClipboard>
#include <QFile>


AboutDialog::AboutDialog(QWidget *parent) : QDialog(parent), ui(new Ui::AboutDialog),
    m_authorsModel(new QStandardItemModel),
    m_modulesModel(new QStandardItemModel),
    m_foldersModel(new QStandardItemModel)
{
    ui->setupUi(this);

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
    ui->textBrowserDescription->append("Copyright 2020-2022 © Enock Nicholaus <font color=\"blue\">3nock@protonmail.com</font> and contributors.");

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

    m_authorsModel->setColumnCount(2);
    m_authorsModel->setHorizontalHeaderLabels({"Author", "Email Contact"});
    ui->tableViewContributors->setModel(m_authorsModel);

    /* loading the authors... */
    this->m_setAuthors();

    ///
    /// for modules...
    ///

    m_modulesModel->setColumnCount(2);
    m_modulesModel->setHorizontalHeaderLabels({"Module", "Module Source"});
    ui->tableViewModules->setModel(m_modulesModel);

    /* loading the modules... */
    this->m_setModules();

    ///
    /// for folders...
    ///

    m_foldersModel->setColumnCount(2);
    m_foldersModel->setHorizontalHeaderLabels({"File", "File Location"});
    ui->tableViewFolders->setModel(m_foldersModel);

    /* loading the folders... */
    this->m_setFolders();

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
    delete m_authorsModel;
    delete m_modulesModel;
    delete m_foldersModel;
    delete ui;
}

void AboutDialog::on_buttonCopy_clicked(){
    QClipboard *clipboard = QGuiApplication::clipboard();
    QString description = ui->textBrowserDescription->toPlainText();
    clipboard->setText(description);
}

void AboutDialog::m_setAuthors(){
    /* for now i am the only author */
    m_authorsModel->appendRow({new QStandardItem("Enock Nicholaus"), new QStandardItem("3nock@protonmail.com")});

    /* later on store authors in a .ini file and load them */
}

void AboutDialog::m_setModules(){
    /* nothing yet for now */
}

void AboutDialog::m_setFolders(){
    /* nothing yet for now */
}

