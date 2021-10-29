#ifndef HOSTNAMECHECKER_H
#define HOSTNAMECHECKER_H

#include <QDialog>

namespace Ui {
class HostnameChecker;
}

class HostnameChecker : public QDialog
{
    Q_OBJECT

public:
    explicit HostnameChecker(QWidget *parent = nullptr);
    ~HostnameChecker();

private:
    Ui::HostnameChecker *ui;
};

#endif // HOSTNAMECHECKER_H
