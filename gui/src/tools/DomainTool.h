#ifndef HOSTNAMETOOL_H
#define HOSTNAMETOOL_H

#include <QDialog>

namespace Ui {
class DomainTool;
}

class DomainTool : public QDialog
{
    Q_OBJECT

public:
    explicit DomainTool(QWidget *parent = nullptr);
    ~DomainTool();

private:
    Ui::DomainTool *ui;
};

#endif // HOSTNAMETOOL_H
