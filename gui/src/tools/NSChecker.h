#ifndef NSCHECKER_H
#define NSCHECKER_H

#include <QDialog>

namespace Ui {
class NSChecker;
}

class NSChecker : public QDialog
{
    Q_OBJECT

public:
    explicit NSChecker(QWidget *parent = nullptr);
    ~NSChecker();

private:
    Ui::NSChecker *ui;
};

#endif // NSCHECKER_H
