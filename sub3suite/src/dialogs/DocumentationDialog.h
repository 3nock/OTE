#ifndef DOCUMENTATIONDIALOG_H
#define DOCUMENTATIONDIALOG_H

#include <QDialog>
#include "src/utils/utils.h"


namespace Ui {
class DocumentationDialog;
}

class DocumentationDialog : public QDialog{
        Q_OBJECT

    public:
        DocumentationDialog(QWidget *parent = nullptr); // for general documentation
        DocumentationDialog(TOOL engine, QWidget *parent = nullptr); // documentation for engines
        DocumentationDialog(ENUMERATOR enumerator, QWidget *parent = nullptr); // documentation for enumerators
        ~DocumentationDialog();

    private slots:
        void on_buttonOk_clicked();
        void on_buttonBack_clicked();

    private:
        Ui::DocumentationDialog *ui;
};

#endif // DOCUMENTATIONDIALOG_H
