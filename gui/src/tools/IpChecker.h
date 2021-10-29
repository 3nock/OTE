#ifndef IPCHECKER_H
#define IPCHECKER_H

#include <QDialog>

namespace Ui {
class IpChecker;
}

class IpChecker : public QDialog
{
    Q_OBJECT

public:
    explicit IpChecker(QWidget *parent = nullptr);
    ~IpChecker();

private:
    Ui::IpChecker *ui;
};

#endif // IPCHECKER_H
