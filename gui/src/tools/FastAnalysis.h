#ifndef FASTANALYSIS_H
#define FASTANALYSIS_H

#include <QDialog>

namespace Ui {
class FastAnalysis;
}

class FastAnalysis : public QDialog
{
    Q_OBJECT

public:
    explicit FastAnalysis(QWidget *parent = nullptr);
    ~FastAnalysis();

private:
    Ui::FastAnalysis *ui;
};

#endif // FASTANALYSIS_H
