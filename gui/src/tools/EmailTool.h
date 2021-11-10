#ifndef EMAILTOOL_H
#define EMAILTOOL_H

#include <QDialog>

namespace Ui {
class EmailTool;
}

class EmailTool : public QDialog
{
    Q_OBJECT

public:
    explicit EmailTool(QWidget *parent = nullptr);
    ~EmailTool();

private:
    Ui::EmailTool *ui;
};

#endif // EMAILCHECKER_H
