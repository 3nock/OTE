#ifndef MXCHECKER_H
#define MXCHECKER_H

#include <QDialog>

namespace Ui {
class MXChecker;
}

class MXChecker : public QDialog
{
    Q_OBJECT

public:
    explicit MXChecker(QWidget *parent = nullptr);
    ~MXChecker();

private:
    Ui::MXChecker *ui;
};

#endif // MXCHECKER_H
