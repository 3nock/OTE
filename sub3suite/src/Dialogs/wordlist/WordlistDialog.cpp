#include "WordlistDialog.h"
#include "ui_WordlistDialog.h"


WordListDialog::WordListDialog(QWidget *parent, QStringListModel *wordlistModel): QDialog(parent), ui(new Ui::WordListDialog),
      m_wordlistModel(wordlistModel),
      m_customWordlistModel(new QStringListModel)
{
    ui->setupUi(this);

    this->m_initChoose();
    this->m_initGenerate();

    this->adjustSize();
}
WordListDialog::~WordListDialog(){
    delete ui;
    delete m_customWordlistModel;
}

void WordListDialog::on_tabWidget_currentChanged(int index){
    Q_UNUSED(index);

    /* adjust the dialog size accordingly */
    ui->tabWidget->adjustSize();
    this->adjustSize();
}
