#ifndef EMAILCHECKER_H
#define EMAILCHECKER_H

#include <QDialog>

namespace Ui {
class EmailChecker;
}

class EmailChecker : public QDialog
{
    Q_OBJECT

public:
    explicit EmailChecker(QWidget *parent = nullptr);
    ~EmailChecker();

private:
    Ui::EmailChecker *ui;
};

#endif // EMAILCHECKER_H
