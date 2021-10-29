#ifndef ASNCHECKER_H
#define ASNCHECKER_H

#include <QDialog>

namespace Ui {
class ASNChecker;
}

class ASNChecker : public QDialog
{
    Q_OBJECT

public:
    explicit ASNChecker(QWidget *parent = nullptr);
    ~ASNChecker();

private:
    Ui::ASNChecker *ui;
};

#endif // ASNCHECKER_H
