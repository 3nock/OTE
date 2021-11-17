#include "Raw.h"
#include "ui_Raw.h"
//...
#include <QClipboard>
#include <QFileDialog>


/********************************************************************
 *                              Actions
 ********************************************************************/

void Raw::onClear(){
    ui->plainTextEdit->clear();
}

void Raw::onCopy(){
    ///
    /// checks...
    ///
    if(ui->plainTextEdit->toPlainText().isEmpty())
        return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->plainTextEdit->toPlainText());
}

void Raw::onSave(){
    ///
    /// checks...
    ///
    if(ui->plainTextEdit->toPlainText().isEmpty())
        return;

    QString filename = QFileDialog::getSaveFileName(this, "Save To File", "./");
    if(filename.isEmpty()){
        return;
    }
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if(file.isOpen())
    {
        file.write(ui->plainTextEdit->toPlainText().toUtf8());
        file.close();
    }
}
