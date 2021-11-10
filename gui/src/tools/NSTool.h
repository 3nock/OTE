#ifndef NSTOOL_H
#define NSTOOL_H

#include <QDialog>

namespace Ui {
class NSTool;
}

class NSTool : public QDialog
{
    Q_OBJECT

public:
    explicit NSTool(QWidget *parent = nullptr);
    ~NSTool();

private:
    Ui::NSTool *ui;
};

#endif // NSTOOL_H
