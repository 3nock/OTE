#ifndef CERTCHECKER_H
#define CERTCHECKER_H

#include <QDialog>

namespace Ui {
class CertChecker;
}

class CertChecker : public QDialog
{
    Q_OBJECT

public:
    explicit CertChecker(QWidget *parent = nullptr);
    ~CertChecker();

private:
    Ui::CertChecker *ui;
};

#endif // CERTCHECKER_H
