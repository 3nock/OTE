#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"


WordListDialog::WordListDialog(QWidget *parent, QStringListModel *wordlistModel): QDialog(parent), ui(new Ui::WordListDialog),
      m_wordlistModel(wordlistModel),
      m_customWordlistModel(new QStringListModel),
      m_generateWordlistModel(new QStringListModel)
{
    ui->setupUi(this);

    this->m_initChoose();
    this->m_initGenerate();

    /* adjust the dialog size accordingly */
    ui->toolBox->hide();
    this->adjustSize();
}
WordListDialog::~WordListDialog(){
    delete ui;
    delete m_customWordlistModel;
}

void WordListDialog::on_tabWidget_currentChanged(int index){
    Q_UNUSED(index);

    /* adjust the dialog size accordingly */
    switch (index) {
    case 0: // Choose Tab
        ui->toolBox->hide();
        ui->groupBoxChoose->show();
        break;
    case 1: // Generate Tab
        ui->toolBox->show();
        ui->groupBoxChoose->hide();
    }

    ui->tabGenerate->adjustSize();
    ui->tabChoose->adjustSize();
    ui->tabWidget->adjustSize();
    this->adjustSize();
}
