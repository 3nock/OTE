#ifndef WORDLISTGENERATORDIALOG_H
#define WORDLISTGENERATORDIALOG_H

#include <QDialog>

namespace Ui {
    class WordListGeneratorDialog;
}

class WordListGeneratorDialog : public QDialog{
        Q_OBJECT

    public:
        explicit WordListGeneratorDialog(QWidget *parent = nullptr);
        ~WordListGeneratorDialog();

    private:
        Ui::WordListGeneratorDialog *ui;
};

#endif // WORDLISTGENERATORDIALOG_H
